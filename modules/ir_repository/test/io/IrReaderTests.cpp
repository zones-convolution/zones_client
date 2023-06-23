#include "ir_repository/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>
static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;

TEST_CASE ("ir_reader can read from disk", "[IrReader]")
{
    SECTION ("reads valid metadata")
    {
        static const std::string kIrIdentifier = "valid_ir";
        auto ir_metadata = IrReader::ReadIrMetadata (kTestDataDirectory, kIrIdentifier);
        REQUIRE (ir_metadata.name == "valid_name");
        REQUIRE (ir_metadata.description == "valid_description");
    }
}