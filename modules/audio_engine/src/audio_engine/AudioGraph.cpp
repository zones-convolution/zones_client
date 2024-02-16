#include "AudioGraph.h"

AudioGraph::AudioGraph (AudioGraphMetering & input_graph_metering,
                        AudioGraphMetering & output_graph_metering)
    : input_graph_metering_ (input_graph_metering)
    , output_graph_metering_ (output_graph_metering)
{
    dry_wet_mixer_.setWetMixProportion (0.5f);
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    dry_wet_mixer_.prepare (spec);
    input_graph_metering_.Prepare (spec.numChannels);
    output_graph_metering_.Prepare (spec.numChannels);
    non_uniform_partitioned_convolver_.prepare (spec);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    output_block.multiplyBy (input_gain_);
    input_graph_metering_.UpdateChannelPeak (input_block);

    dry_wet_mixer_.pushDrySamples (replacing.getInputBlock ());

    if (is_ready_)
        non_uniform_partitioned_convolver_.process (replacing);

    dry_wet_mixer_.mixWetSamples (replacing.getOutputBlock ());
    output_block.multiplyBy (output_gain_);
    output_graph_metering_.UpdateChannelPeak (input_block);
}

void AudioGraph::reset ()
{
    dry_wet_mixer_.reset ();
}

void AudioGraph::operator() (const CommandQueue::LoadIr & load_ir)
{
    non_uniform_partitioned_convolver_.LoadImpulseResponse (*load_ir.ir_buffer,
                                                            load_ir.sample_rate);
    is_ready_ = true;

    delete load_ir
        .ir_buffer; // ABSOLUTELY HORRIFIC BUT STOPS MEMORY LEAK FOR NOW WHILE USING JUCE'S CONV...
}

void AudioGraph::operator() (const CommandQueue::UpdateParameters & update_parameters)
{
    dry_wet_mixer_.setWetMixProportion (update_parameters.dry_wet_mix);
    input_gain_ = update_parameters.input_gain;
    output_gain_ = update_parameters.output_gain;
}
