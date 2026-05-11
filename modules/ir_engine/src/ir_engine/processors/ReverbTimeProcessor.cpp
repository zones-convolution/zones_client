#include "ReverbTimeProcessor.h"

void ReverbTimeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                   juce::AudioBuffer<float> & output_buffer,
                                   const IrGraphState & state)
{
    auto buf = input_buffer;
    juce::dsp::AudioBlock<const float> input_block {*buf};

    auto num_samples = input_block.getNumSamples ();
    auto end_sample = std::max ((int) std::floor ((float) num_samples * state.reverb_time_norm), 1);

    output_buffer.setSize (input_buffer->getNumChannels (), end_sample);
    juce::dsp::AudioBlock<float> output_block {output_buffer};
    output_block.copyFrom (input_block);

    if (end_sample == num_samples)
        return;

    auto gain_curve = [&] (float sample_index)
    {
        auto delta = 1.f / (float) end_sample;
        return 1.f - std::pow (delta * sample_index, 2);
    };

    for (auto channel_index = 0; channel_index < output_block.getNumChannels (); ++channel_index)
    {
        auto channel_block = output_block.getChannelPointer (channel_index);
        for (auto sample_index = 0; sample_index < output_block.getNumSamples (); ++sample_index)
        {
            channel_block [sample_index] *= gain_curve (sample_index);
        }
    }
}
