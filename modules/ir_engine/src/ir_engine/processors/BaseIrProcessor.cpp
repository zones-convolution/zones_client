#include "BaseIrProcessor.h"

#include "ResamplerProcessor.h"

static void NormaliseImpulseResponse (juce::AudioBuffer<float> & output_buffer)
{
    auto num_channels = output_buffer.getNumChannels ();
    auto num_samples = output_buffer.getNumSamples ();

    auto max_rms = 0.0f;
    for (auto i = 0; i < num_channels; ++i)
        max_rms = std::max (max_rms, output_buffer.getRMSLevel (i, 0, num_samples));

    auto norm_factor = 1.0f / output_buffer.getMagnitude (0, num_samples);

    norm_factor *= 0.05f / max_rms;
    norm_factor *= std::min (1.2f, 48000.0f / static_cast<float> (num_samples));
    norm_factor *= juce::Decibels::decibelsToGain (-12.0f);
    norm_factor = std::clamp (norm_factor, 0.0f, 4.0f);

    juce::dsp::AudioBlock<float> {output_buffer}.multiplyBy (norm_factor);
}

void BaseIrProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                               juce::AudioBuffer<float> & output_buffer,
                               const IrGraphState & state)
{
    auto ratio = 1.f;

    if (state.sample_rate > 0)
        ratio = static_cast<float> (state.base_ir_sample_rate / state.sample_rate);

    ResamplerProcessor::ResampleBuffer (
        state.base_ir_buffer.get (), output_buffer, ratio, state.base_ir_sample_rate);
    NormaliseImpulseResponse (output_buffer);
}