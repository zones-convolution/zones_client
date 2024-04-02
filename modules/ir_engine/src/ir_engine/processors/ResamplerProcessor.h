#pragma once
#include "../IrGraphProcessor.h"

#include <juce_dsp/juce_dsp.h>

class ResamplerProcessor : public IrGraphProcessor
{
public:
    ResamplerProcessor ();

    void Process (BoxedBuffer & input_buffer,
                  juce::AudioBuffer<float> & output_buffer,
                  const IrGraphState & state) override;
    ~ResamplerProcessor () override = default;

private:
    std::vector<std::unique_ptr<juce::LagrangeInterpolator>> interpolators_;
};
