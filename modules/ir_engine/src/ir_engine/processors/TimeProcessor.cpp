#include "TimeProcessor.h"

void TimeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                             juce::AudioBuffer<float> & output_buffer,
                             const IrGraphState & state)
{
    auto input_num_samples = input_buffer->getNumSamples ();

    auto num_samples_to_trim =
        std::min (juce::roundToInt (state.trim_norm * input_num_samples), input_num_samples - 1);

    auto trimmed_num_samples = input_num_samples - num_samples_to_trim;
    auto attack_num_samples = juce::roundToInt (state.attack_norm * trimmed_num_samples);

    auto input_sub_block =
        juce::dsp::AudioBlock<const float> {*input_buffer}.getSubBlock (num_samples_to_trim);

    auto num_samples_of_predelay = ((state.pre_delay_ms) / 1000.f) * state.sample_rate;

    output_buffer.setSize (input_buffer->getNumChannels (),
                           trimmed_num_samples + num_samples_of_predelay);
    output_buffer.clear ();

    for (auto sample_index = 0; sample_index < attack_num_samples; ++sample_index)
    {
        auto attack_gain =
            (1.0f / static_cast<float> (attack_num_samples)) * static_cast<float> (sample_index);
        for (auto channel_index = 0; channel_index < output_buffer.getNumChannels ();
             ++channel_index)
        {
            auto sample = input_sub_block.getSample (channel_index, sample_index) * attack_gain;
            output_buffer.setSample (channel_index, sample_index + num_samples_of_predelay, sample);
        }
    }

    if (trimmed_num_samples > attack_num_samples)
    {
        juce::dsp::AudioBlock<float> {output_buffer}
            .getSubBlock (num_samples_of_predelay + attack_num_samples)
            .copyFrom (input_sub_block.getSubBlock (attack_num_samples));
    }
}