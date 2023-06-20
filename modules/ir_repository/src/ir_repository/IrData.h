#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

struct IrMetadata
{
};

struct IrData
{
    juce::AudioBuffer<float> impulse_response;
    IrMetadata metadata;
};