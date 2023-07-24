#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
    dry_wet_mixer_.setWetMixProportion (0.5f);
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    dry_wet_mixer_.prepare (spec);
    processor_chain_.prepare (spec);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    dry_wet_mixer_.pushDrySamples (replacing.getInputBlock ());
    processor_chain_.process (replacing);
    dry_wet_mixer_.mixWetSamples (replacing.getOutputBlock ());
}

void AudioGraph::reset ()
{
    dry_wet_mixer_.reset ();
    processor_chain_.reset ();
}

void AudioGraph::operator() (const CommandQueue::LoadIr & load_ir)
{
    auto & convolver = processor_chain_.get<0> ();
    convolver.loadImpulseResponse (std::move (*load_ir.ir_buffer),
                                   load_ir.sample_rate,
                                   juce::dsp::Convolution::Stereo::yes,
                                   juce::dsp::Convolution::Trim::no,
                                   juce::dsp::Convolution::Normalise::yes);
}

void AudioGraph::operator() (const CommandQueue::UpdateParameters & update_parameters)
{
    dry_wet_mixer_.setWetMixProportion (update_parameters.dry_wet_mix);
}
