#include "ir_repository/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>
static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;

static const std::string kValidIrIdentifier = "valid_ir";
static const std::string kNonExistentIr = "this_does_not_exist";
static const std::string kInvalidIrFile = "invalid_ir";

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
        IrData ir_data;
        IrReader::ReadIrData (kTestDataDirectory, kValidIrIdentifier, ir_data);

        REQUIRE (ir_data.buffer.getNumChannels () > 0);
        REQUIRE (ir_data.buffer.getNumSamples () == 288000);

        REQUIRE (ir_data.bit_depth == 24);
        REQUIRE (ir_data.sample_rate == 96000);
    }

    SECTION ("throws NoIrFileException when Ir file does not exist")
    {
        IrData ir_data;
        REQUIRE_THROWS_AS (IrReader::ReadIrData (kTestDataDirectory, kNonExistentIr, ir_data),
                           IrReader::NoIrFileException);
    }

    SECTION ("throws FailedToReadIrException when Wav file is invalid")
    {
        IrData ir_data;
        REQUIRE_THROWS_AS (IrReader::ReadIrData (kTestDataDirectory, kInvalidIrFile, ir_data),
                           IrReader::FailedToReadIrException);
    }
}
