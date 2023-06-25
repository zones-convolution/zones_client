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

void ScopedIrIO::ReadIr (const std::string & ir_identifier, juce::AudioBuffer<float> & ir_buffer)
{
    IrReader::ReadIr (load_path_, ir_identifier, ir_buffer);
}