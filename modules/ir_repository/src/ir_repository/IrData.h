#pragma once

#include "IrMetadata.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>

struct IrData
{
    juce::AudioBuffer<float> buffer;
    double sample_rate;
    double bit_depth;
};

class IrDataFormat
{
public:
    static std::filesystem::path
    GetMetadataFileNameForIdentifier (const std::string & ir_identifier);
    static std::filesystem::path
    GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier);

private:
    static const std::string kMetadataExtension;
    static const std::string kImpulseResponseExtension;
};