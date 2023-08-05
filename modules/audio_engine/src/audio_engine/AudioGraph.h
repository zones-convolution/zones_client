#pragma once

#include "AudioGraphMetering.h"
#include "CommandQueue.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph
    : public juce::dsp::ProcessorBase
    , public CommandQueue::Visitor
{
public:
    explicit AudioGraph (AudioGraphMetering & audio_graph_metering);
    ~AudioGraph () override = default;
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void operator() (const CommandQueue::LoadIr & load_ir) override;
    void operator() (const CommandQueue::UpdateParameters & update_parameters) override;

private:
    struct NonUniformConvolver : public juce::dsp::Convolution
    {
        static constexpr int kRequiredHeadSize = 1024;
        NonUniformConvolver ()
            : juce::dsp::Convolution {juce::dsp::Convolution::NonUniform {kRequiredHeadSize}}
        {
        }
    };

    AudioGraphMetering & audio_graph_metering_;
    juce::dsp::DryWetMixer<float> dry_wet_mixer_;
    juce::dsp::ProcessorChain<NonUniformConvolver> processor_chain_;
};