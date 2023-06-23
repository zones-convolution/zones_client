#pragma once

#include "IrMetadata.h"

#include <juce_audio_basics/juce_audio_basics.h>

struct IrData
{
    juce::AudioBuffer<float> impulse_response;
    IrMetadata metadata;
};