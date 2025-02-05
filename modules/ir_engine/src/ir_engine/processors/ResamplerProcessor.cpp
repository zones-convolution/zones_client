#include "ResamplerProcessor.h"

void ResamplerProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                  juce::AudioBuffer<float> & output_buffer,
                                  const IrGraphState & state)
{
    auto ratio = state.resampler / 100.f;
    jassert (ratio > 0.49f && ratio < 1.51f);

    auto sample_rate = state.sample_rate > 0.f ? state.sample_rate : state.base_ir_sample_rate;
    ResampleBuffer (input_buffer, output_buffer, ratio, sample_rate, true);
}

void ResamplerProcessor::ResampleBuffer (const juce::AudioBuffer<float> & input_buffer,
                                         juce::AudioBuffer<float> & output_buffer,
                                         const float ratio,
                                         const float input_sample_rate,
                                         const bool useLagrangeInterpolator)
{
    jassert (input_sample_rate > 0.f);
    if (ratio == 1.f)
    {
        output_buffer.makeCopyOf (input_buffer);
        return;
    }

    auto num_channels = input_buffer.getNumChannels ();
    auto output_num_samples = static_cast<float> (input_buffer.getNumSamples ()) / ratio;
    output_buffer.setSize (num_channels, static_cast<int> (std::ceil (output_num_samples)));

    if (useLagrangeInterpolator)
    {
        juce::AudioBuffer<float> temp_input (input_buffer.getNumChannels (),
                                             input_buffer.getNumSamples ());
        temp_input.makeCopyOf (input_buffer);

        juce::IIRFilter lowPassFilter;
        auto low_pass_cutoff =
            (ratio > 1.f) ? (input_sample_rate / ratio) * 0.5f * 0.9f : input_sample_rate * 0.5f;
        lowPassFilter.setCoefficients (
            juce::IIRCoefficients::makeLowPass (input_sample_rate, low_pass_cutoff));

        juce::LagrangeInterpolator interpolator;

        for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
        {
            lowPassFilter.reset ();
            lowPassFilter.processSamples (temp_input.getWritePointer (channel_index),
                                          temp_input.getNumSamples ());

            interpolator.reset ();
            interpolator.process (ratio,
                                  temp_input.getReadPointer (channel_index),
                                  output_buffer.getWritePointer (channel_index),
                                  output_num_samples);
        }
    }
    else
    {
        juce::WindowedSincInterpolator interpolator;

        for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
        {
            interpolator.reset ();
            interpolator.process (ratio,
                                  input_buffer.getReadPointer (channel_index),
                                  output_buffer.getWritePointer (channel_index),
                                  output_num_samples);
        }
    }
}
