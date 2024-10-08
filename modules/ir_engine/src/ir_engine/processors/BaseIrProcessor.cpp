#include "BaseIrProcessor.h"

static void NormaliseImpulseResponse (juce::AudioBuffer<float> & output_buffer)
{
    auto num_channels = output_buffer.getNumChannels ();
    auto num_samples = output_buffer.getNumSamples ();

    auto max_rms = 0.0f;
    for (auto i = 0; i < num_channels; ++i)
        max_rms = std::max (max_rms, output_buffer.getRMSLevel (i, 0, num_samples));

    auto norm_factor = 1.0f / output_buffer.getMagnitude (0, num_samples);

    norm_factor *= 0.125f / max_rms;
    norm_factor *= std::min (1.2f, 48000.0f / static_cast<float> (num_samples));
    norm_factor *= juce::Decibels::decibelsToGain (-12.0f);
    norm_factor = std::clamp (norm_factor, 0.0f, 4.0f);

    juce::dsp::AudioBlock<float> {output_buffer}.multiplyBy (norm_factor);
}

void BaseIrProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                               juce::AudioBuffer<float> & output_buffer,
                               const IrGraphState & state)
{
    output_buffer.makeCopyOf (state.base_ir_buffer.get ());
    NormaliseImpulseResponse (output_buffer);
}