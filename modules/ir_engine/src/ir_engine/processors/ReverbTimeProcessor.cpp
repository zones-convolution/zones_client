#include "ReverbTimeProcessor.h"

void ReverbTimeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                   juce::AudioBuffer<float> & output_buffer,
                                   const IrGraphState & state)
{
    juce::dsp::AudioBlock<const float> input_block {*input_buffer};
    output_buffer.setSize (input_buffer->getNumChannels (), input_buffer->getNumSamples ());
    juce::dsp::AudioBlock<float> output_block {output_buffer};
    output_block.copyFrom (input_block);

    auto reverb_time = 0.8f;
    auto num_samples = input_block.getNumSamples ();
    auto fade_start = (int) std::floor ((float) num_samples * reverb_time);
    auto fade_samples = num_samples - fade_start;

    auto fade_block = output_block.getSubBlock (fade_start);
    for (auto channel_index = 0; channel_index < fade_block.getNumChannels (); ++channel_index)
    {
        for (auto sample = 0; sample < fade_block.getNumSamples (); ++sample)
        {
        }
    }
}
