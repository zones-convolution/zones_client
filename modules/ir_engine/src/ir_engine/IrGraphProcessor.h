#pragma once
#include "juce_dsp/juce_dsp.h"

struct IrGraphState
{
    float param_1;
    int param_2;
    std::string param_3;
};

class IrGraphCachePolicy
{
public:
    [[nodiscard]] virtual std::size_t GetHashForState (const IrGraphState & state) const = 0;
    virtual ~IrGraphCachePolicy () = default;
};

class IrGraphProcessor
{
public:
    virtual void Process (juce::dsp::ProcessContextNonReplacing<float> & process_context,
                          const IrGraphState & state) = 0;
    virtual ~IrGraphProcessor () = default;
};
