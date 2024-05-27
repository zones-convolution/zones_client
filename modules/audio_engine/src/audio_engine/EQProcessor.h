#pragma once
#include <juce_dsp/juce_dsp.h>

class EQProcessor : public juce::dsp::ProcessorBase
{
public:
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void UpdateFilters (float bass, float treble);

private:
    using filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                                  juce::dsp::IIR::Coefficients<float>>;
    juce::dsp::ProcessorChain<filter, filter> processor_chain_;
    juce::dsp::ProcessSpec spec_;

    const float q_factor_ = 1.f;
    const float low_cutoff = 1000.f;
    const float high_cutoff = 1500.f;
};
