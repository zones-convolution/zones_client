#pragma once

#include "../IrGraphProcessor.h"

class TestProcessor : public IrGraphProcessor
{
public:
    void Process (juce::dsp::ProcessContextNonReplacing<float> & process_context,
                  const IrGraphState & state) override;
};
