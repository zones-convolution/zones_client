#pragma once
#include "../IrData.h"

#include <filesystem>

class IrWriter
{
public:
    static void WriteIrMetadata (const std::filesystem::path & write_path,
                                 const std::string & ir_identifier,
                                 const IrMetadata & ir_metadata);
};
