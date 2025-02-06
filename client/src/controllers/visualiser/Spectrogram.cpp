#include "Spectrogram.h"

static constexpr float kMaxIrRatio = 2.0f;
static constexpr float kResolution = 1024.0f;
static constexpr float kReferenceSampleRate = 48000.0f;
static constexpr int kTargetFFTSize = 1024;
static constexpr int kTargetNumNonNegativeFFTPoints = kTargetFFTSize / 2;

static int GetFFTSize (int hop_size, double base_sample_rate)
{
    auto samples_per_hop = static_cast<int> (std::round (hop_size / kResolution));
    return std::max (static_cast<int> (juce::nextPowerOfTwo (2 * samples_per_hop)), kTargetFFTSize);
}

static int GetHopSize (int base_num_samples, double base_sample_rate)
{
    auto max_num_samples_frac = (kMaxIrRatio * base_num_samples) + (0.5 * base_sample_rate);
    return static_cast<int> (std::round (max_num_samples_frac / kResolution));
}

static juce::AudioBuffer<float> PerformFFT (const juce::dsp::AudioBlock<const float> & audio_block,
                                            int base_num_samples,
                                            double base_sample_rate)
{
    auto hop_size = GetHopSize (base_num_samples, base_sample_rate);
    auto fft_size = GetFFTSize (hop_size, base_sample_rate);
    auto fft_order = static_cast<int> (std::log2 (fft_size));

    juce::AudioBuffer<float> fft_data {1, fft_size * 2};
    juce::dsp::AudioBlock<float> fft_block {fft_data};
    juce::dsp::FFT fft {fft_order};
    juce::dsp::WindowingFunction<float> window {static_cast<size_t> (fft_size),
                                                juce::dsp::WindowingFunction<float>::hann};

    auto num_samples = static_cast<int> (audio_block.getNumSamples ());
    auto num_hops = static_cast<int> (
        std::ceil (static_cast<float> (num_samples) / static_cast<float> (hop_size)));

    juce::AudioBuffer<float> frequency_data {
        num_hops, (fft_size / 2)}; // Should actually have a + 1 here I think
    juce::dsp::AudioBlock<float> frequency_block {frequency_data};
    frequency_block.clear ();

    for (auto hop_index = 0; hop_index < num_hops; ++hop_index)
    {
        auto sample_index = hop_index * hop_size;
        auto block_size = std::min (fft_size, num_samples - sample_index);
        auto sub_block = audio_block.getSubBlock (sample_index, block_size);

        fft_block.clear ();
        for (auto channel_index = 0; channel_index < sub_block.getNumChannels (); ++channel_index)
            fft_block.add (sub_block.getSingleChannelBlock (channel_index));

        window.multiplyWithWindowingTable (fft_block.getChannelPointer (0), fft_size);
        fft.performFrequencyOnlyForwardTransform (fft_block.getChannelPointer (0), true);
        frequency_block.getSingleChannelBlock (hop_index).copyFrom (
            fft_block.getSubBlock (0, (fft_size / 2)));
    }

    return frequency_data;
}

static void NormaliseFrequencyData (juce::dsp::AudioBlock<float> frequency_block)
{
    auto num_channels = frequency_block.getNumChannels ();
    auto num_samples = frequency_block.getNumSamples ();

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        for (auto sample_index = 0; sample_index < num_samples; ++sample_index)
        {
            auto scaled_level =
                std::log10 (frequency_block.getSample (channel_index, sample_index) + 1.f);
            frequency_block.setSample (channel_index, sample_index, scaled_level);
        }
    }

    auto level_range = frequency_block.findMinAndMax ();
    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        for (auto sample_index = 0; sample_index < num_samples; ++sample_index)
        {
            auto normalised_level =
                juce::jmap (frequency_block.getSample (channel_index, sample_index),
                            level_range.getStart (),
                            std::max (level_range.getEnd (), 1e-5f),
                            0.0f,
                            1.0f);
            normalised_level = std::clamp (normalised_level, 0.f, 1.f);
            frequency_block.setSample (channel_index, sample_index, normalised_level);
        }
    }
}

static juce::AudioBuffer<float>
AverageFrequencyData (const juce::dsp::AudioBlock<const float> & frequency_data)
{
    auto out_y_dim = frequency_data.getNumSamples ();
    auto num_channels = frequency_data.getNumChannels ();

    juce::AudioBuffer<float> averaged_frequency_data;
    averaged_frequency_data.setSize (num_channels, kTargetFFTSize / 2);

    juce::LinearInterpolator interpolator;

    auto ratio = static_cast<float> (kTargetFFTSize) / static_cast<float> (out_y_dim / 2);

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        interpolator.reset ();
        interpolator.process (ratio,
                              frequency_data.getChannelPointer (channel_index),
                              averaged_frequency_data.getWritePointer (channel_index),
                              kTargetNumNonNegativeFFTPoints);
    }

    return averaged_frequency_data;
}

static juce::AudioBuffer<std::uint8_t>
ConvertToUint8Buffer (const juce::AudioBuffer<float> & buffer)
{
    auto num_channels = buffer.getNumChannels ();
    auto num_samples = buffer.getNumSamples ();

    juce::AudioBuffer<std::uint8_t> data {num_channels, num_samples};
    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
        for (auto sample_index = 0; sample_index < num_samples; ++sample_index)
            data.setSample (channel_index,
                            sample_index,
                            static_cast<std::uint8_t> (
                                std::floor (buffer.getSample (channel_index, sample_index) * 255)));

    return data;
}

Spectrogram::BoxedUint8Buffer
Spectrogram::CreateNormalisedSpectrogramData (Spectrogram::BoxedBuffer buffer,
                                              int base_num_sample,
                                              double base_sample_rate)
{
    auto frequency_data = PerformFFT (*buffer, base_num_sample, base_sample_rate);
    if (frequency_data.getNumSamples () > kTargetNumNonNegativeFFTPoints)
        frequency_data = AverageFrequencyData (frequency_data);
    //    NormaliseFrequencyData (frequency_data);
    return {ConvertToUint8Buffer (frequency_data)};
}