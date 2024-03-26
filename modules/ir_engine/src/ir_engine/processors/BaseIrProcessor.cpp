#include "BaseIrProcessor.h"

static float CalculateNormalisationFactor (float sum_squared_magnitude) // COPIED FROM JUCE CONV
{
    if (sum_squared_magnitude < 1e-8f)
        return 1.0f;

    return 0.125f / std::sqrt (sum_squared_magnitude);
}

static void
NormaliseImpulseResponse (juce::AudioBuffer<float> & output_buffer) // COPIED FROM JUCE CONV
{
    const auto num_channels = output_buffer.getNumChannels ();
    const auto num_samples = output_buffer.getNumSamples ();
    const auto channel_ptrs = output_buffer.getArrayOfWritePointers ();

    const auto max_sum_squared_mag =
        std::accumulate (channel_ptrs,
                         channel_ptrs + num_channels,
                         0.0f,
                         [num_samples] (auto max, auto * channel)
                         {
                             return juce::jmax (max,
                                                std::accumulate (channel,
                                                                 channel + num_samples,
                                                                 0.0f,
                                                                 [] (auto sum, auto samp)
                                                                 { return sum + (samp * samp); }));
                         });

    const auto normalisation_factor = CalculateNormalisationFactor (max_sum_squared_mag);

    std::for_each (
        channel_ptrs,
        channel_ptrs + num_channels,
        [normalisation_factor, num_samples] (auto * channel)
        { juce::FloatVectorOperations::multiply (channel, normalisation_factor, num_samples); });
}

void BaseIrProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                               juce::AudioBuffer<float> & output_buffer,
                               const IrGraphState & state)
{
    output_buffer.makeCopyOf (state.base_ir_buffer.get ());
    NormaliseImpulseResponse (output_buffer);
}