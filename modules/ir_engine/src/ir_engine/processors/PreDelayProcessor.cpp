#include "PreDelayProcessor.h"

void PreDelayProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                 juce::AudioBuffer<float> & output_buffer,
                                 const IrGraphState & state)
{
    auto num_samples_of_predelay = ((state.pre_delay) / 1000.f) * state.sample_rate;

    output_buffer.setSize (input_buffer->getNumChannels (),
                           input_buffer->getNumSamples () + num_samples_of_predelay);
    output_buffer.clear ();

    juce::dsp::AudioBlock<float> {output_buffer}
        .getSubBlock (num_samples_of_predelay)
        .copyFrom (*input_buffer);
}
