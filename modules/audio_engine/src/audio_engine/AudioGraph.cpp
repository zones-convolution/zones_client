#include "AudioGraph.h"

#include <juce_audio_basics/juce_audio_basics.h>

AudioGraph::AudioGraph (AudioGraphMetering & input_graph_metering,
                        AudioGraphMetering & output_graph_metering,
                        zones::ConvolutionEngine & convolution_engine,
                        NotificationQueue::VisitorQueue & notification_queue,
                        ParameterTree & parameter_tree)
    : notification_queue_ (notification_queue)
    , input_graph_metering_ (input_graph_metering)
    , output_graph_metering_ (output_graph_metering)
    , parameter_tree_ (parameter_tree)
    , player_processor_ (notification_queue)
    , convolution_engine_ (convolution_engine)
{
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    dry_wet_mixer_.prepare (spec);
    convolution_engine_.prepare (spec);
    input_graph_metering_.Prepare (spec.numChannels);
    output_graph_metering_.Prepare (spec.numChannels);

    player_processor_.prepare (spec);
    eq_processor_.prepare (spec);
    convolution_engine_.reset ();
    // This will need more checks but is used for now to stop some
    // potential crashes?? would clear be better?

    smoothed_input_gain_.reset (spec.sampleRate, 0.1f);
    smoothed_output_gain_.reset (spec.sampleRate, 0.1f);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    UpdateParameters ();

    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    output_block.multiplyBy (smoothed_input_gain_);

    player_processor_.process (replacing);

    input_graph_metering_.UpdateChannelPeak (input_block);

    dry_wet_mixer_.pushDrySamples (replacing.getInputBlock ());

    convolution_engine_.process (replacing);

    eq_processor_.process (replacing.getOutputBlock ());

    dry_wet_mixer_.mixWetSamples (replacing.getOutputBlock ());
    output_block.multiplyBy (smoothed_output_gain_);
    output_graph_metering_.UpdateChannelPeak (output_block);
}

void AudioGraph::reset ()
{
    dry_wet_mixer_.reset ();
    convolution_engine_.reset ();
    eq_processor_.reset ();
}

void AudioGraph::operator() (const CommandQueue::SetPlayerStateCommand & set_player_state_command)
{
    player_processor_.SetPlayerState ({set_player_state_command.state});
}

void AudioGraph::UpdateParameters ()
{
    smoothed_input_gain_.setTargetValue (
        juce::Decibels::decibelsToGain<float> (*parameter_tree_.input_gain_parameter));

    smoothed_output_gain_.setTargetValue (
        juce::Decibels::decibelsToGain<float> (*parameter_tree_.out_gain_parameter));

    auto dry_wet_parameter = parameter_tree_.dry_wet_parameter;
    auto dry_wet_norm = dry_wet_parameter->convertTo0to1 (*dry_wet_parameter);
    dry_wet_mixer_.setWetMixProportion (dry_wet_norm);

    //    eq_processor_.UpdateFilters (update_parameters.bass, update_parameters.treble);
}
