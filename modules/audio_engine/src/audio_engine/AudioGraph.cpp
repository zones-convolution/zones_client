#include "AudioGraph.h"

AudioGraph::AudioGraph (AudioGraphMetering & input_graph_metering,
                        AudioGraphMetering & output_graph_metering,
                        zones::ConvolutionEngine & convolution_engine)
    : input_graph_metering_ (input_graph_metering)
    , output_graph_metering_ (output_graph_metering)
    , convolution_engine_ (convolution_engine)
{
    dry_wet_mixer_.setWetMixProportion (0.5f);
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    dry_wet_mixer_.prepare (spec);
    convolution_engine_.prepare (spec);
    input_graph_metering_.Prepare (spec.numChannels);
    output_graph_metering_.Prepare (spec.numChannels);

    convolution_engine_.Clear (); // This will need more checks but is used for now to stop some
                                  // potential crashes??
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    output_block.multiplyBy (input_gain_);
    input_graph_metering_.UpdateChannelPeak (input_block);

    dry_wet_mixer_.pushDrySamples (replacing.getInputBlock ());

    convolution_engine_.process (replacing);

    dry_wet_mixer_.mixWetSamples (replacing.getOutputBlock ());
    output_block.multiplyBy (output_gain_);
    output_graph_metering_.UpdateChannelPeak (input_block);
}

void AudioGraph::reset ()
{
    dry_wet_mixer_.reset ();
    convolution_engine_.reset ();
}

void AudioGraph::operator() (const CommandQueue::UpdateParameters & update_parameters)
{
    dry_wet_mixer_.setWetMixProportion (update_parameters.dry_wet_mix);
    input_gain_ = update_parameters.input_gain;
    output_gain_ = update_parameters.output_gain;
}
