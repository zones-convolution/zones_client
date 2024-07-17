#include "ReverbTimeProcessor.h"

void ReverbTimeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                   juce::AudioBuffer<float> & output_buffer,
                                   const IrGraphState & state)
{
    auto buf = input_buffer;
    juce::dsp::AudioBlock<const float> input_block {*buf};
    output_buffer.setSize (input_buffer->getNumChannels (), input_buffer->getNumSamples ());
    juce::dsp::AudioBlock<float> output_block {output_buffer};
    output_block.copyFrom (input_block);

    auto num_samples = input_block.getNumSamples ();
    auto fade_start = (int) std::floor ((float) num_samples * state.reverb_time_norm);
    auto fade_samples = num_samples - fade_start;

    auto gain_curve = [&] (float sample_index)
    {
        auto delta = 1.f / (float) fade_samples;
        return 1.f - std::pow (delta * sample_index, 2);
    };

    if (fade_start == num_samples)
        return;

    auto fade_block = output_block.getSubBlock (fade_start);
    for (auto channel_index = 0; channel_index < fade_block.getNumChannels (); ++channel_index)
    {
        auto channel_block = fade_block.getChannelPointer (channel_index);
        for (auto sample_index = 0; sample_index < fade_block.getNumSamples (); ++sample_index)
        {
            channel_block [sample_index] *= gain_curve (sample_index);
        }
    }
}
