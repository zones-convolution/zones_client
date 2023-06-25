#include "IrData.h"

const std::string IrDataFormat::kMetadataExtension = ".json";
const std::string IrDataFormat::kImpulseResponseExtension = ".wav";

std::filesystem::path
IrDataFormat::GetMetadataFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kMetadataExtension;
}

std::filesystem::path
IrDataFormat::GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kImpulseResponseExtension;
}