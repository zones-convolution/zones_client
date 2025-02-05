#pragma once
#include "../IrGraphProcessor.h"

#include <juce_dsp/juce_dsp.h>

class ResamplerProcessor : public IrGraphProcessor
{
public:
    void Process (BoxedBuffer & input_buffer,
                  juce::AudioBuffer<float> & output_buffer,
                  const IrGraphState & state) override;

    static void ResampleBuffer (const juce::AudioBuffer<float> & input_buffer,
                                juce::AudioBuffer<float> & output_buffer,
                                const float ratio,
                                const float input_sample_rate,
                                const bool useLagrangeInterpolator = false);
    ~ResamplerProcessor () override = default;
};
