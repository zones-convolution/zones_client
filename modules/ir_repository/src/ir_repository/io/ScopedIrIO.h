#pragma once

#include "IrReader.h"
#include "IrWriter.h"

#include <filesystem>
#include <lager/reader.hpp>

class ScopedIrIO
{
public:
    explicit ScopedIrIO (std::filesystem::path load_path);

    [[nodiscard]] IrMetadata ReadIrMetadata (const std::string & ir_identifier);
    void ReadIrData (const std::string & ir_identifier, IrData & ir_data);

    void WriteIrMetadata (const std::string & ir_identifier, const IrMetadata & ir_metadata);
    void WriteIrData (const std::string & ir_identifier, const IrData & ir_data);

private:
    const std::filesystem::path load_path_;
};
