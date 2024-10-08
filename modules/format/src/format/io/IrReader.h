#pragma once

#include "../IrData.h"
#include "../ZoneMetadata.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

#include <filesystem>
#include <immer/flex_vector.hpp>
#include <optional>
#include <variant>

class IrReader
{
public:
    virtual void ReadIrData (const std::filesystem::path & load_path,
                             const std::string & ir_identifier,
                             IrData & ir_data);

    struct NoMetadataFileException : std::exception
    {
    };

    struct NoIrFileException : std::exception
    {
    };

    struct FailedToReadIrException : std::exception
    {
    };

private:
    static void ReadAudioFileToIrData (const std::filesystem::path & audio_path, IrData & ir_data);
};
