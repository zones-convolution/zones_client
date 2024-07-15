#include "TimeProcessor.h"

void TimeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                             juce::AudioBuffer<float> & output_buffer,
                             const IrGraphState & state)
{
    auto input_num_samples = input_buffer->getNumSamples ();
    auto num_samples_to_remove = juce::roundToInt (state.trim * input_num_samples);

    num_samples_to_remove = std::min (num_samples_to_remove, input_num_samples - 1);

    juce::dsp::AudioBlock<const float> input_block {*input_buffer};
    juce::dsp::AudioBlock<const float> input_sub_block =
        input_block.getSubBlock (num_samples_to_remove);

    auto num_samples_of_predelay = ((state.pre_delay) / 1000.f) * state.sample_rate;

    output_buffer.setSize (input_buffer->getNumChannels (),
                           input_sub_block.getNumSamples () + num_samples_of_predelay);
    output_buffer.clear ();

    juce::dsp::AudioBlock<float> {output_buffer}
        .getSubBlock (num_samples_of_predelay)
        .copyFrom (input_sub_block);
}
