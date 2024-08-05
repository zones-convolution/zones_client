#include "ResamplerProcessor.h"

void ResamplerProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                  juce::AudioBuffer<float> & output_buffer,
                                  const IrGraphState & state)
{
    juce::LagrangeInterpolator interpolator;

    auto ratio = state.resampler / 100.f;
    jassert (ratio > 0.49f && ratio < 1.51f);

    auto num_channels = input_buffer->getNumChannels ();

    auto output_num_samples = static_cast<float> (input_buffer->getNumSamples ()) / ratio;
    output_buffer.setSize (num_channels, static_cast<int> (std::ceil (output_num_samples)));

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        interpolator.reset ();
        interpolator.process (ratio,
                              input_buffer->getReadPointer (channel_index),
                              output_buffer.getWritePointer (channel_index),
                              output_num_samples);
    }
}
