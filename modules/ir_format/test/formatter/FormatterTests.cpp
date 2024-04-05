#include "ir_format/formatter/MonoFormatter.h"
#include "ir_format/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>

static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;
static std::filesystem::path kMonoTestDataDirectory =
    kTestDataDirectory / std::filesystem::path {"Mono"};

TEST_CASE ("mono formatter correctly loads file", "[MonoFormatter]")
{
    IrFormatData ir_format_data {.channel_format = ChannelFormat::kMono,
                                 .position_map = PositionMap {.centre = "C"}};

    IrReader ir_reader;
    IrData correct_ir_data;
    ir_reader.ReadIrData (kMonoTestDataDirectory, "C", correct_ir_data);

    IrData ir_data;
    MonoFormatter::Format (kMonoTestDataDirectory, ir_format_data, TargetFormat::kMono, ir_data);

    REQUIRE (ir_data.buffer.getNumChannels () == correct_ir_data.buffer.getNumChannels ());
    REQUIRE (ir_data.buffer.getNumSamples () == correct_ir_data.buffer.getNumSamples ());

    REQUIRE (ir_data.bit_depth == correct_ir_data.bit_depth);
    REQUIRE (ir_data.sample_rate == correct_ir_data.sample_rate);
}