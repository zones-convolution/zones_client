#include "AudioGraph.h"

AudioGraph::AudioGraph ()
{
}

void AudioGraph::prepare (const juce::dsp::ProcessSpec & spec)
{
    processor_chain_.prepare (spec);
}

void AudioGraph::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    processor_chain_.process (replacing);
}

void AudioGraph::reset ()
{
    processor_chain_.reset ();
}

void AudioGraph::RTLoadIr (const IrData * ir_data)
{
    juce::AudioBuffer<float> ir_buffer {};
    ir_buffer.setDataToReferTo (
        const_cast<float * const *> (ir_data->buffer.getArrayOfReadPointers ()),
        ir_data->buffer.getNumChannels (),
        ir_data->buffer.getNumSamples ()); // SUPER SUPER BAD JUST TO GET THIS WORKING...

    auto & convolver = processor_chain_.get<0> ();
    convolver.loadImpulseResponse (std::move (ir_buffer),
                                   ir_data->sample_rate,
                                   juce::dsp::Convolution::Stereo::yes,
                                   juce::dsp::Convolution::Trim::no,
                                   juce::dsp::Convolution::Normalise::no);
}

void AudioGraph::RTUpdateParameters ()
{
}
