#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    processor_chain_.prepare (spec);
    auto gain = processor_chain_.get<1> ();
    gain.setGainLinear (.8f);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    processor_chain_.process (replacing);
}

void AudioGraph::reset ()
{
    processor_chain_.reset ();
}

void AudioGraph::RTLoadIr (IrData && ir_data)
{
    auto & convolver = processor_chain_.get<0> ();
    convolver.loadImpulseResponse (std::move (ir_data.buffer),
                                   ir_data.sample_rate,
                                   juce::dsp::Convolution::Stereo::yes,
                                   juce::dsp::Convolution::Trim::no,
                                   juce::dsp::Convolution::Normalise::no);
}

void AudioGraph::RTUpdateParameters ()
{
}
