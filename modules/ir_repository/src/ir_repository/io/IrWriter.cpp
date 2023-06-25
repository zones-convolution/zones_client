#include "IrWriter.h"

void IrWriter::WriteIrMetadata (const std::filesystem::path & write_path,
                                const std::string & ir_identifier,
                                const IrMetadata & ir_metadata)
{
    auto file_path = write_path / IrDataFormat::GetMetadataFileNameForIdentifier (ir_identifier);
    auto write_file = juce::File (file_path.string ());
    if (! write_file.getParentDirectory ().exists ())
        throw DirectoryDoesNotExistException {};

    auto output_stream = write_file.createOutputStream ();
    IrMetadata::ToDynamic (ir_metadata).writeAsJSON (*output_stream, 0, true, 4);
}
