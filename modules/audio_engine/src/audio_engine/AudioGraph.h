#pragma once

#include "AudioGraphMetering.h"
#include "CommandQueue.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph
    : public juce::dsp::ProcessorBase
    , public CommandQueue::Visitor
{
public:
    explicit AudioGraph (AudioGraphMetering & input_graph_metering,
                         AudioGraphMetering & output_graph_metering,
                         ConvolutionEngine & convolution_engine);
    ~AudioGraph () override = default;
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void operator() (const CommandQueue::UpdateParameters & update_parameters) override;

private:
    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;

    juce::dsp::DryWetMixer<float> dry_wet_mixer_;
    ConvolutionEngine & convolution_engine_;

    float input_gain_ = 1.f;
    float output_gain_ = 1.f;
};