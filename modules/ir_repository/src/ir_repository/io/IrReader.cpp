#include "IrReader.h"

const std::string IrReader::kMetadataExtension = ".zone_meta";
const std::string IrReader::kImpulseResponseExtension = ".wav";

std::string IrReader::GetMetadataFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kMetadataExtension;
}

std::string IrReader::GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kImpulseResponseExtension;
}

IrData IrReader::ReadIrData (const std::filesystem::path & load_path,
                             const std::string & ir_identifier)
{
    juce::ignoreUnused (load_path, ir_identifier);
    return {};
}