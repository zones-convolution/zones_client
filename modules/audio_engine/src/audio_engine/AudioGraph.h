#pragma once
#include "CommandQueue.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph
    : public juce::dsp::ProcessorBase
    , public CommandQueue::Delegate
{
public:
    explicit AudioGraph ();
    ~AudioGraph () override = default;
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void RTLoadIr (IrData && ir_data) override;
    void RTUpdateParameters () override;

private:
    struct NonUniformConvolver : public juce::dsp::Convolution
    {
        static constexpr int kRequiredHeadSize = 1024;
        NonUniformConvolver ()
            : juce::dsp::Convolution {juce::dsp::Convolution::NonUniform {kRequiredHeadSize}}
        {
        }
    };

    juce::dsp::ProcessorChain<NonUniformConvolver, juce::dsp::Gain<float>> processor_chain_;
};