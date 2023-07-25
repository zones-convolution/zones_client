#pragma once
#include "../IrGraphProcessor.h"

#include <juce_dsp/juce_dsp.h>

class ReverbTimeProcessor : public IrGraphProcessor
{
public:
    void Process (BoxedBuffer & input_buffer,
                  juce::AudioBuffer<float> & output_buffer,
                  const IrGraphState & state) override;
    ~ReverbTimeProcessor () override = default;

private:
};
