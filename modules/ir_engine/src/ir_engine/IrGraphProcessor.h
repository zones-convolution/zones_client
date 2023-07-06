#pragma once
#include "immer/box.hpp"
#include "juce_dsp/juce_dsp.h"

struct IrGraphState
{
    float param_1;
    int param_2;
    std::string param_3;

    static float CacheParam1 (const IrGraphState & state)
    {
        return state.param_1;
    }

    static int CacheParam2 (const IrGraphState & state)
    {
        return state.param_2;
    }

    static const std::string & CacheParam3 (const IrGraphState & state)
    {
        return state.param_3;
    }
};

class IrGraphProcessor
{
public:
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;

    virtual void Process (BoxedBuffer & input_buffer,
                          juce::AudioBuffer<float> & output_buffer,
                          const IrGraphState & state) = 0;
    virtual ~IrGraphProcessor () = default;
};
