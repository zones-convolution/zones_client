
#include "ir_repository/io/IrReader.h"
#include "ir_repository/io/IrWriter.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir writer can write to disk", "[IrWriter]")
{
    SECTION ("writes valid metadata")
    {
        IrMetadata write_ir_metadata {.name = "name", .description = "description"};
        auto temp_dir =
            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)
                .getFullPathName ()
                .toStdString ();
        auto ir_identifier = juce::Uuid ().toString ().toStdString ();
        IrWriter::WriteIrMetadata (temp_dir, ir_identifier, write_ir_metadata);

        auto read_ir_metadata = IrReader::ReadIrMetadata (temp_dir, ir_identifier);
        REQUIRE (read_ir_metadata.name == write_ir_metadata.name);
        REQUIRE (read_ir_metadata.description == write_ir_metadata.description);

        auto absolute_path =
            temp_dir / IrDataFormat::GetMetadataFileNameForIdentifier (ir_identifier);
        juce::File (absolute_path.string ()).moveToTrash ();
    }

    SECTION ("fails to write when directory does not exist")
    {
        IrMetadata write_ir_metadata {.name = "name", .description = "description"};
        auto temp_dir = "./this/directory/does/not/exist/hopefully";
        auto ir_identifier = juce::Uuid ().toString ().toStdString ();

        REQUIRE_THROWS_AS (IrWriter::WriteIrMetadata (temp_dir, ir_identifier, write_ir_metadata),
                           IrWriter::DirectoryDoesNotExistException);
    }
}