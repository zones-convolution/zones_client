#include "ResamplerProcessor.h"

ResamplerProcessor::ResamplerProcessor ()
{
    auto num_channels = 2;
    for (auto i = 0; i < num_channels; ++i)
    {
        interpolators_.push_back (std::make_unique<juce::LagrangeInterpolator> ());
    }
}

void ResamplerProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                  juce::AudioBuffer<float> & output_buffer,
                                  const IrGraphState & state)
{
    auto buf = input_buffer;
    juce::dsp::AudioBlock<const float> input_block {*buf};

    auto num_channels = input_block.getNumChannels ();

    auto ratio = state.resampler_ratio;
    if (ratio <= 0.f)
        return;

    auto output_num_samples = input_buffer->getNumSamples () / ratio;
    output_buffer.setSize (num_channels, output_num_samples);
    juce::dsp::AudioBlock<float> output_block {output_buffer};

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        interpolators_ [channel_index]->reset ();
        interpolators_ [channel_index]->process (ratio,
                                                 input_buffer->getReadPointer (channel_index),
                                                 output_buffer.getWritePointer (channel_index),
                                                 output_num_samples);
    }
}
