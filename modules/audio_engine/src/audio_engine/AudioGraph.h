#pragma once

#include "AudioGraphMetering.h"
#include "CommandQueue.h"
#include "EQProcessor.h"
#include "NotificationQueue.h"
#include "model/ParameterTree.h"
#include "player/PlayerProcessor.h"
#include "zones_convolver/zones_convolver.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class AudioGraph : public CommandQueue::Visitor
{
public:
    explicit AudioGraph (AudioGraphMetering & input_graph_metering,
                         AudioGraphMetering & output_graph_metering,
                         zones::ConvolutionEngine & convolution_engine,
                         NotificationQueue::VisitorQueue & notification_queue,
                         ParameterTree & parameter_tree);
    void Prepare (const juce::dsp::ProcessSpec & spec,
                  const juce::AudioProcessor::BusesLayout & layout);
    void Process (const juce::dsp::ProcessContextReplacing<float> & replacing);
    void Reset ();

    void operator() (const CommandQueue::SetPlayerStateCommand & set_player_state_command) override;

private:
    void UpdateParameters ();

    NotificationQueue::VisitorQueue & notification_queue_;
    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;
    ParameterTree parameter_tree_;

    PlayerProcessor player_processor_;
    juce::dsp::DryWetMixer<float> dry_wet_mixer_;
    zones::ConvolutionEngine & convolution_engine_;
    EQProcessor eq_processor_;

    juce::SmoothedValue<float> smoothed_input_gain_;
    juce::SmoothedValue<float> smoothed_output_gain_;
};