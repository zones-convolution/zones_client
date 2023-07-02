#pragma once

#include "../IrGraphProcessor.h"

class TestCachePolicy : public IrGraphCachePolicy
{
public:
    std::size_t GetHashForState (const IrGraphState & state) const override;
};

class TestProcessor : public IrGraphProcessor
{
public:
    void Process (juce::dsp::ProcessContextNonReplacing<float> & process_context) override;
};
