#pragma once
#include "immer/box.hpp"
#include "juce_dsp/juce_dsp.h"

struct IrGraphState;

class IrGraphProcessor
{
public:
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;

    virtual void Process (BoxedBuffer & input_buffer,
                          juce::AudioBuffer<float> & output_buffer,
                          const IrGraphState & state) = 0;
    virtual ~IrGraphProcessor () = default;
};

struct IrGraphState
{
    float param_1;
    int param_2;
    std::string param_3;
    std::string base_ir;
    IrGraphProcessor::BoxedBuffer base_ir_buffer;

    float room_size;

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

    static const std::string & CacheBaseIr (const IrGraphState & state)
    {
        return state.base_ir;
    }

    static const float CacheRoomSize (const IrGraphState & state)
    {
        return state.room_size;
    }
};