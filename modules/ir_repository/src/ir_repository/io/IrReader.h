#pragma once

#include "../IrData.h"

#include <filesystem>
#include <juce_core/juce_core.h>
#include <optional>
#include <variant>

class IrReader
{
public:
    [[nodiscard]] static IrData ReadIrData (const std::filesystem::path & load_path,
                                            const std::string & ir_identifier);

    [[nodiscard]] static juce::AudioBuffer<float> ReadIr (const std::filesystem::path & load_path,
                                                          const std::string & ir_identifier);

    [[nodiscard]] static juce::AudioBuffer<float>
    ReadIrMetadata (const std::filesystem::path & load_path, const std::string & ir_identifier);

private:
    [[nodiscard]] static std::string
    GetMetadataFileNameForIdentifier (const std::string & ir_identifier);
    [[nodiscard]] static std::string
    GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier);

    static const std::string kMetadataExtension;
    static const std::string kImpulseResponseExtension;
};
