#pragma once

#include "../IrData.h"
#include "../IrMetadata.h"
#include "juce_audio_formats/juce_audio_formats.h"

#include <filesystem>

class IrWriter
{
public:
    virtual void WriteIrMetadata (const std::filesystem::path & write_path,
                                  const std::string & ir_identifier,
                                  const IrMetadata & ir_metadata);
    virtual void WriteIrData (const std::filesystem::path & write_path,
                              const std::string & ir_identifier,
                              const IrData & ir_data);

    struct DirectoryDoesNotExistException : std::exception
    {
    };
};
