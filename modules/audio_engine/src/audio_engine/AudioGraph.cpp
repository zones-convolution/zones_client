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

void AudioGraph::operator() (const CommandQueue::LoadIr & load_ir)
{
    juce::AudioBuffer<float> ir_buffer {};
    ir_buffer.setDataToReferTo (load_ir.ir_data->buffer.getArrayOfWritePointers (),
                                load_ir.ir_data->buffer.getNumChannels (),
                                load_ir.ir_data->buffer.getNumSamples ());

    auto & convolver = processor_chain_.get<0> ();
    convolver.loadImpulseResponse (std::move (ir_buffer),
                                   load_ir.ir_data->sample_rate,
                                   juce::dsp::Convolution::Stereo::yes,
                                   juce::dsp::Convolution::Trim::no,
                                   juce::dsp::Convolution::Normalise::no);
}

void AudioGraph::operator() (const CommandQueue::UpdateParameters & update_parameters)
{
}
