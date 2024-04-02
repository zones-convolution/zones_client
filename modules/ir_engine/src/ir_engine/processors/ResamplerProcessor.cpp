#include "ResamplerProcessor.h"

void ResamplerProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                  juce::AudioBuffer<float> & output_buffer,
                                  const IrGraphState & state)
{
    auto ratio = state.resampler_ratio;
    jassert (ratio > 0.1f && ratio < 2.f);

    auto buf = input_buffer;
    auto num_channels = buf->getNumChannels ();

    auto output_num_samples = input_buffer->getNumSamples () / ratio;
    output_buffer.setSize (num_channels, output_num_samples);

    juce::LagrangeInterpolator interpolator;

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        interpolator.reset ();
        interpolator.process (ratio,
                              input_buffer->getReadPointer (channel_index),
                              output_buffer.getWritePointer (channel_index),
                              output_num_samples);
    }
}
