#include "ir_repository/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>
static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;

static const std::string kValidIrIdentifier = "valid_ir";
static const std::string kNonExistentIr = "this_does_not_exist";

TEST_CASE ("ir_reader can read from disk", "[IrReader]")
{
    SECTION ("reads valid metadata")
    {
        auto ir_metadata = IrReader::ReadIrMetadata (kTestDataDirectory, kValidIrIdentifier);
        REQUIRE (ir_metadata.name == "valid_name");
        REQUIRE (ir_metadata.description == "valid_description");
    }

    SECTION ("throws NoMetadataFileException when metadata file does not exist")
    {
        REQUIRE_THROWS_AS (IrReader::ReadIrMetadata (kTestDataDirectory, kNonExistentIr),
                           IrReader::NoMetadataFileException);
    }

    SECTION ("reads valid ir")
    {
        juce::AudioBuffer<float> ir_buffer;
        IrReader::ReadIr (kTestDataDirectory, kValidIrIdentifier, ir_buffer);
        REQUIRE (ir_buffer.getNumChannels () > 0);
        REQUIRE (ir_buffer.getNumSamples () == 288000);
    }

    SECTION ("throws NoIrFileException when Ir file does not exist")
    {
        juce::AudioBuffer<float> ir_buffer;
        REQUIRE_THROWS_AS (IrReader::ReadIr (kTestDataDirectory, kNonExistentIr, ir_buffer),
                           IrReader::NoIrFileException);
    }
}