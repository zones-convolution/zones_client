#include "ScopedIrIO.h"

#include <utility>

ScopedIrIO::ScopedIrIO (std::filesystem::path load_path)
    : load_path_ (std::move (load_path))
{
}

IrMetadata ScopedIrIO::ReadIrMetadata (const std::string & ir_identifier)
{
    return IrReader::ReadIrMetadata (load_path_, ir_identifier);
}

void ScopedIrIO::ReadIrData (const std::string & ir_identifier, IrData & ir_data)
{
    IrReader::ReadIrData (load_path_, ir_identifier, ir_data);
}

void ScopedIrIO::WriteIrMetadata (const std::string & ir_identifier, const IrMetadata & ir_metadata)
{
    IrWriter::WriteIrMetadata (load_path_, ir_identifier, ir_metadata);
}

void ScopedIrIO::WriteIrData (const std::string & ir_identifier, const IrData & ir_data)
{
    IrWriter::WriteIrData (load_path_, ir_identifier, ir_data);
}
