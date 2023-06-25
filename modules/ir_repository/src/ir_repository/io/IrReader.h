#pragma once

#include "../IrData.h"

#include <filesystem>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <optional>
#include <variant>

class IrReader
{
public:
    static void ReadIrData (const std::filesystem::path & load_path,
                            const std::string & ir_identifier,
                            IrData & ir_data);

    static void ReadIr (const std::filesystem::path & load_path,
                        const std::string & ir_identifier,
                        juce::AudioBuffer<float> & ir_buffer);

    [[nodiscard]] static IrMetadata ReadIrMetadata (const std::filesystem::path & load_path,
                                                    const std::string & ir_identifier);

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
    static void ReadAudioFileToBuffer (const std::filesystem::path & audio_path,
                                       juce::AudioBuffer<float> & audio_buffer);

    [[nodiscard]] static std::filesystem::path
    GetMetadataFileNameForIdentifier (const std::string & ir_identifier);
    [[nodiscard]] static std::filesystem::path
    GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier);

    static const std::string kMetadataExtension;
    static const std::string kImpulseResponseExtension;
};
