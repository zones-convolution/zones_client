#include "format/formatter/MonoFormatter.h"
#include "format/formatter/StereoFormatter.h"
#include "format/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>

static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;
static std::filesystem::path kMonoTestDataDirectory =
    kTestDataDirectory / std::filesystem::path {"mono"};
static std::filesystem::path kStereoTestDataDirectory =
    kTestDataDirectory / std::filesystem::path {"stereo"};

static void RequirePreservesSamplingData (const IrData & a, const IrData & b)
{
    REQUIRE (a.bit_depth == b.bit_depth);
    REQUIRE (a.sample_rate == b.sample_rate);
}

TEST_CASE ("mono formatter correctly loads files", "[MonoFormatter]")
{
    IrFormatData ir_format_data {.channel_format = ChannelFormat::kMono,
                                 .position_map = PositionMap {.centre = "c.wav"}};

    IrReader ir_reader;
    IrData correct_ir_data;
    ir_reader.ReadIrData (kMonoTestDataDirectory, "c.wav", correct_ir_data);

    IrData ir_data;
    MonoFormatter::Format (kMonoTestDataDirectory, ir_format_data, TargetFormat::kMono, ir_data);

    REQUIRE (ir_data.buffer.getNumChannels () == 1);
    REQUIRE (ir_data.buffer.getNumSamples () == correct_ir_data.buffer.getNumSamples ());

    RequirePreservesSamplingData (correct_ir_data, ir_data);
}

TEST_CASE ("stereo formatter correctly loads files", "[StereoFormatter]")
{
    IrReader ir_reader;
    IrData correct_ir_data_L;
    ir_reader.ReadIrData (kStereoTestDataDirectory, "l.wav", correct_ir_data_L);
    IrData correct_ir_data_C;
    ir_reader.ReadIrData (kStereoTestDataDirectory, "c.wav", correct_ir_data_C);
    IrData correct_ir_data_R;
    ir_reader.ReadIrData (kStereoTestDataDirectory, "r.wav", correct_ir_data_R);

    IrFormatData ir_format_data {
        .channel_format = ChannelFormat::kStereo,
        .position_map = PositionMap {.centre = "c.wav", .left = "l.wav", .right = "r.wav"}};

    SECTION ("loading for a mono target")
    {
        IrData ir_data;
        StereoFormatter::Format (
            kStereoTestDataDirectory, ir_format_data, TargetFormat::kMono, ir_data);

        REQUIRE (ir_data.buffer.getNumChannels () == 1);
        REQUIRE (ir_data.buffer.getNumSamples () == correct_ir_data_C.buffer.getNumSamples ());

        // should be included if bit depth and sample rate should be copied
        //        REQUIRE (ir_data.bit_depth == correct_ir_data_C.bit_depth);
        //        REQUIRE (ir_data.sample_rate == correct_ir_data_C.sample_rate);
    }

    SECTION ("loading for a stereo target")
    {
        IrData ir_data;
        StereoFormatter::Format (
            kStereoTestDataDirectory, ir_format_data, TargetFormat::kStereo, ir_data);

        REQUIRE (ir_data.buffer.getNumChannels () == 2);
        REQUIRE (ir_data.buffer.getNumSamples () == correct_ir_data_C.buffer.getNumSamples ());
    }

    SECTION ("loading for a true stereo target")
    {
        IrData ir_data;
        StereoFormatter::Format (
            kStereoTestDataDirectory, ir_format_data, TargetFormat::kTrueStereo, ir_data);

        REQUIRE (ir_data.buffer.getNumChannels () == 4);
        REQUIRE (ir_data.buffer.getNumSamples () == correct_ir_data_L.buffer.getNumSamples ());
    }
}