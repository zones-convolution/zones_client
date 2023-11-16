#pragma once

#include "AudioGraphMetering.h"
#include "CommandQueue.h"
#include "OverlapAddConvolver.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph
    : public juce::dsp::ProcessorBase
    , public CommandQueue::Visitor
{
public:
    explicit AudioGraph (AudioGraphMetering & input_graph_metering,
                         AudioGraphMetering & output_graph_metering);
    ~AudioGraph () override = default;
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void operator() (const CommandQueue::LoadIr & load_ir) override;
    void operator() (const CommandQueue::UpdateParameters & update_parameters) override;

private:
    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;

    juce::dsp::DryWetMixer<float> dry_wet_mixer_;
    juce::dsp::ProcessorChain<OverlapAddConvolver> processor_chain_;
    float input_gain_ = 1.f;
    float output_gain_ = 1.f;
};