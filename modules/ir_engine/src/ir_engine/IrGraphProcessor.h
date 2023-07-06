#pragma once
#include "immer/box.hpp"
#include "juce_dsp/juce_dsp.h"

struct IrGraphState
{
    float param_1;
    int param_2;
    std::string param_3;
};

class IrGraphProcessor
{
public:
    using BoxedBuffer = immer::box<const juce::AudioBuffer<float>>;

    virtual void Process (BoxedBuffer & input_buffer,
                          juce::AudioBuffer<float> & output_buffer,
                          const IrGraphState & state) = 0;
    virtual ~IrGraphProcessor () = default;
};
