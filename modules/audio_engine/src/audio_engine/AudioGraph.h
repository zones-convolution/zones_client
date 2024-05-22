#pragma once

#include "AudioGraphMetering.h"
#include "CommandQueue.h"
#include "NotificationQueue.h"
#include "player/PlayerProcessor.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_dsp/juce_dsp.h>

class AudioGraph
    : public juce::dsp::ProcessorBase
    , public CommandQueue::Visitor
{
public:
    explicit AudioGraph (AudioGraphMetering & input_graph_metering,
                         AudioGraphMetering & output_graph_metering,
                         zones::ConvolutionEngine & convolution_engine,
                         NotificationQueue::VisitorQueue & notification_queue);
    ~AudioGraph () override = default;
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;

    void operator() (const CommandQueue::UpdateParameters & update_parameters) override;
    void operator() (const CommandQueue::PlayCommand & play_command) override;
    void operator() (const CommandQueue::StopCommand & stop_command) override;

private:
    NotificationQueue::VisitorQueue & notification_queue_;
    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;

    PlayerProcessor player_processor_;
    juce::dsp::DryWetMixer<float> dry_wet_mixer_;
    zones::ConvolutionEngine & convolution_engine_;

    float input_gain_ = 1.f;
    float output_gain_ = 1.f;
};