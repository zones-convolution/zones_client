#include "ir_format/io/IrReader.h"
#include "ir_format/io/IrWriter.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir writer can write to disk", "[IrWriter]")
{
    IrWriter ir_writer;
    IrReader ir_reader;

    SECTION ("writes valid metadata")
    {
        IrMetadata write_ir_metadata {.channel_format = ChannelFormat::kFoa,
                                      .position_map =
                                          PositionMap {.centre = "c", .left = "l", .right = "r"},
                                      .name = "name",
                                      .description = "description"};
        auto temp_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)
                .getFullPathName ()
                .toStdString ();
        auto ir_identifier = juce::Uuid ().toString ().toStdString ();
        ir_writer.WriteIrMetadata (temp_dir, ir_identifier, write_ir_metadata);

        auto read_ir_metadata = ir_reader.ReadIrMetadata (temp_dir, ir_identifier);

        REQUIRE (read_ir_metadata.name == write_ir_metadata.name);
        REQUIRE (read_ir_metadata.description == write_ir_metadata.description);
        REQUIRE (read_ir_metadata.channel_format == ChannelFormat::kFoa);

        REQUIRE (read_ir_metadata.position_map->centre == "c");
        REQUIRE (read_ir_metadata.position_map->left == "l");
        REQUIRE (read_ir_metadata.position_map->right == "r");

        auto absolute_path =
            temp_dir / IrDataFormat::GetMetadataFileNameForIdentifier (ir_identifier);
        juce::File (absolute_path.string ()).moveToTrash ();
    }

    SECTION ("fails to write when directory does not exist")
    {
        IrMetadata write_ir_metadata {.name = "name", .description = "description"};
        auto temp_dir = "./this/directory/does/not/exist/hopefully";
        auto ir_identifier = juce::Uuid ().toString ().toStdString ();

        REQUIRE_THROWS_AS (ir_writer.WriteIrMetadata (temp_dir, ir_identifier, write_ir_metadata),
                           IrWriter::DirectoryDoesNotExistException);
    }

    SECTION ("writes ir data")
    {
        IrData write_ir_data {.sample_rate = 44100, .bit_depth = 24};

        static constexpr int kWriteNumChannels = 2;
        static constexpr int kWriteNumSamples = 1024;
        write_ir_data.buffer.setSize (kWriteNumChannels, kWriteNumSamples);
        write_ir_data.buffer.getWritePointer (0) [0] = 1.f;

        auto temp_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)
                .getFullPathName ()
                .toStdString ();
        auto ir_identifier = juce::Uuid ().toString ().toStdString ();

        ir_writer.WriteIrData (temp_dir, ir_identifier, write_ir_data);

        IrData read_ir_data {};
        ir_reader.ReadIrData (temp_dir, ir_identifier, read_ir_data);

        REQUIRE (juce::approximatelyEqual (write_ir_data.sample_rate, read_ir_data.sample_rate));
        REQUIRE (juce::approximatelyEqual (write_ir_data.bit_depth, read_ir_data.bit_depth));

        REQUIRE (read_ir_data.buffer.getNumChannels () == write_ir_data.buffer.getNumChannels ());
        REQUIRE (read_ir_data.buffer.getNumSamples () == write_ir_data.buffer.getNumSamples ());

        REQUIRE (juce::approximatelyEqual (read_ir_data.buffer.getReadPointer (0) [0],
                                           write_ir_data.buffer.getReadPointer (0) [0]));

        auto absolute_path =
            temp_dir / IrDataFormat::GetImpulseResponseFileNameForIdentifier (ir_identifier);
        juce::File (absolute_path.string ()).moveToTrash ();
    }
}