#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

struct Metadata
{
};

struct IrData
{
    juce::AudioBuffer<float> ir;
    Metadata metadata;
};

class ImpulseResponseReader
{
public:
    static IrData ReadIrData (string ir_id, string load_path);
};
