#pragma once

#include "format/IrData.h"

#include <immer/box.hpp>
#include <juce_dsp/juce_dsp.h>

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
    TargetFormat target_format;
    float param_1;
    int param_2;
    std::string param_3;
    std::string base_ir;
    IrGraphProcessor::BoxedBuffer base_ir_buffer;
    double sample_rate;
    double bit_depth;
    float room_size;
    float reverb_time_norm;
    float resampler;
    float pre_delay_ms;
    float trim_norm;
    float attack_norm;

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

    static int CacheTargetFormat (const IrGraphState & state)
    {
        return static_cast<int> (state.target_format);
    }

    static const std::string & CacheBaseIr (const IrGraphState & state)
    {
        return state.base_ir;
    }

    static float CacheRoomSize (const IrGraphState & state)
    {
        return state.room_size;
    }

    static float CacheReverbTime (const IrGraphState & state)
    {
        return state.reverb_time_norm;
    }

    static float CacheResamplerRatio (const IrGraphState & state)
    {
        return state.resampler;
    }

    static float CachePredelay (const IrGraphState & state)
    {
        return state.pre_delay_ms;
    }

    static float CacheTrim (const IrGraphState & state)
    {
        return state.trim_norm;
    }

    static float CacheAttack (const IrGraphState & state)
    {
        return state.attack_norm;
    }
};