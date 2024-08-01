#include "Spectrogram.h"

static constexpr float kMaxIrRatio = 2.0f;
static constexpr float kResolution = 1024.0f;
static constexpr float kReferenceSampleRate = 48000.0f;
static constexpr int kTargetFFTSize = 1024;

static juce::AudioBuffer<float> PerformFFT (const juce::dsp::AudioBlock<const float> & audio_block,
                                            int base_num_samples,
                                            double base_sample_rate)
{
    auto max_num_samples_frac = kMaxIrRatio * base_num_samples;
    auto sample_rate_scale = kReferenceSampleRate / base_sample_rate;
    auto samples_per_hop = static_cast<int> (std::round ((max_num_samples_frac / kResolution)));

    auto fft_size = static_cast<int> (juce::nextPowerOfTwo (2 * samples_per_hop));
    fft_size = std::max (fft_size, kTargetFFTSize);
    auto fft_order = static_cast<int> (std::log2 (fft_size));

    juce::AudioBuffer<float> fft_data {1, fft_size * 2};
    juce::dsp::AudioBlock<float> fft_block {fft_data};
    juce::dsp::FFT fft {fft_order};
    juce::dsp::WindowingFunction<float> window {static_cast<size_t> (fft_size),
                                                juce::dsp::WindowingFunction<float>::hann};

    auto num_samples = static_cast<int> (audio_block.getNumSamples ());
    auto num_hops = static_cast<int> (
        std::ceil (static_cast<float> (num_samples) / static_cast<float> (samples_per_hop)));

    juce::AudioBuffer<float> frequency_data {
        num_hops, (fft_size / 2)}; // Should actually have a + 1 here I think
    juce::dsp::AudioBlock<float> frequency_block {frequency_data};
    frequency_block.clear ();

    for (auto hop_index = 0; hop_index < num_hops; ++hop_index)
    {
        auto sample_index = hop_index * samples_per_hop;
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

static void DrawSpectrogramLine (juce::Image & spectrogram,
                                 const juce::dsp::AudioBlock<const float> frequency_block)
{
    juce::ColourGradient gradient {
        juce::Colours::darkblue, {0.f, 0.f}, juce::Colours::cyan, {1.0f, 1.0f}, false};

    auto height = spectrogram.getHeight ();
    auto right_hand_edge_index = spectrogram.getWidth () - 1;
    auto fft_size = static_cast<int> (frequency_block.getNumSamples ());

    for (auto y = 0; y < height; ++y)
    {
        auto y_normalised = static_cast<float> (y) / static_cast<float> (height);
        auto fft_data_index = juce::jlimit (
            0,
            fft_size - 1,
            static_cast<int> (std::round (y_normalised * static_cast<float> (fft_size))));

        auto level = frequency_block.getSample (0, fft_data_index);
        auto colour = gradient.getColourAtPosition (level);
        spectrogram.setPixelAt (right_hand_edge_index, (height - 1) - y, colour);
    }
}

static juce::Image DrawSpectrogramImage (const juce::dsp::AudioBlock<const float> & frequency_block)
{
    auto width = static_cast<int> (frequency_block.getNumChannels ());
    auto height = static_cast<int> (frequency_block.getNumSamples ());

    juce::Image spectrogram {juce::Image::RGB, width, height, true};

    for (auto x = 0; x < width; ++x)
    {
        DrawSpectrogramLine (spectrogram, frequency_block.getSingleChannelBlock (x));
        spectrogram.moveImageSection (0, 0, 1, 0, width, height);
    }

    return spectrogram;
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
                              kTargetFFTSize / 2);
    }

    return averaged_frequency_data;
}

Spectrogram::BoxedBuffer
Spectrogram::CreateNormalisedSpectrogramData (Spectrogram::BoxedBuffer buffer,
                                              int base_num_sample,
                                              double base_sample_rate)
{
    auto frequency_data = PerformFFT (*buffer, base_num_sample, base_sample_rate);

    if (frequency_data.getNumSamples () >
        (kTargetFFTSize /
         2)) // Divied by 2 here because frquency only with no negative is half size...
        frequency_data = AverageFrequencyData (frequency_data);
    NormaliseFrequencyData (frequency_data);

    auto spec_image = DrawSpectrogramImage (frequency_data);

    auto png_file =
        juce::File ("/Users/leonps/Documents/development/zones_client/client/zones_spec.png");
    png_file.moveToTrash ();
    juce::FileOutputStream stream (png_file);

    juce::PNGImageFormat png_writer;
    png_writer.writeImageToStream (spec_image, stream);

    return {std::move (frequency_data)};
}