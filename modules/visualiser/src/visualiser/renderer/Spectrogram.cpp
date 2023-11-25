#include "Spectrogram.h"

#include <tinycolormap.hpp>

static juce::Colour ConvertToJuceColour (const tinycolormap::Color & color)
{
    return juce::Colour::fromFloatRGBA (color.r (), color.g (), color.b (), 1.f);
}

static void DrawSpectrogramLine (juce::Image & spectrogram,
                                 juce::dsp::AudioBlock<float> frequency_block)
{
    auto height = spectrogram.getHeight ();
    auto right_hand_edge_index = spectrogram.getWidth () - 1;
    auto fft_size = static_cast<int> (frequency_block.getNumSamples ());

    for (auto y = 0; y < height; ++y)
    {
        auto y_normalised = static_cast<float> (y) / static_cast<float> (height);
        auto y_skewed = std::exp (std::log (0.2f * y_normalised));
        auto fft_data_index =
            juce::jlimit (0, fft_size - 1, static_cast<int> (std::round (y_skewed * fft_size)));

        auto level = frequency_block.getSample (0, fft_data_index);
        auto colour = ConvertToJuceColour (
                          tinycolormap::GetColor (level, tinycolormap::ColormapType::Viridis))
                          .withAlpha (level);
        spectrogram.setPixelAt (right_hand_edge_index, (height - 1) - y, colour);
    }
}

juce::AudioBuffer<float> PerformFFT (const juce::dsp::AudioBlock<const float> & audio_block,
                                     std::size_t fft_order)
{
    auto fft_size = 1u << fft_order;
    juce::AudioBuffer<float> fft_data {1, 2 * static_cast<int> (fft_size)};
    juce::dsp::AudioBlock<float> fft_block {fft_data};

    juce::dsp::FFT fft {static_cast<int> (fft_order)};
    juce::dsp::WindowingFunction<float> window {fft_size,
                                                juce::dsp::WindowingFunction<float>::hann};

    auto num_samples = static_cast<int> (audio_block.getNumSamples ());
    auto hop_length = fft_size / 2;
    auto num_hops = num_samples / hop_length;

    juce::AudioBuffer<float> frequency_data {static_cast<int> (num_hops),
                                             static_cast<int> ((fft_size / 2) + 1)};
    juce::dsp::AudioBlock<float> frequency_block {frequency_data};
    frequency_block.clear ();

    for (auto hop_index = 0u; hop_index < num_hops; ++hop_index)
    {
        auto sample_index = hop_index * hop_length;
        auto block_size = std::min (fft_size, num_samples - sample_index);
        auto sub_block = audio_block.getSubBlock (sample_index, block_size);

        fft_block.clear ();
        for (auto channel_index = 0; channel_index < sub_block.getNumChannels (); ++channel_index)
            fft_block.add (sub_block.getSingleChannelBlock (channel_index));

        window.multiplyWithWindowingTable (fft_block.getChannelPointer (0), fft_size);
        fft.performFrequencyOnlyForwardTransform (fft_block.getChannelPointer (0), true);
        frequency_block.getSingleChannelBlock (hop_index).copyFrom (
            fft_block.getSubBlock (0, (fft_size / 2) + 1));
    }

    return {std::move (frequency_data)};
}

void NormaliseFrequencyData (juce::dsp::AudioBlock<float> frequency_block)
{
    auto level_range = frequency_block.findMinAndMax ();
    for (auto sample_index = 0; sample_index < frequency_block.getNumSamples (); ++sample_index)
    {
        auto normalised_level = juce::jmap (frequency_block.getSample (0, sample_index),
                                            level_range.getStart (),
                                            std::max (level_range.getEnd (), 1e-5f),
                                            0.0f,
                                            1.0f);
        frequency_block.setSample (0, sample_index, normalised_level);
    }
}

juce::Image Spectrogram::CreateSpectrogram (const juce::dsp::AudioBlock<const float> & audio_block)
{
    static constexpr auto kFFTOrder = 10u;

    auto frequency_data = PerformFFT (audio_block, kFFTOrder);
    juce::dsp::AudioBlock<float> frequency_block {frequency_data};

    NormaliseFrequencyData (frequency_block);

    auto width = frequency_data.getNumChannels ();
    auto height = frequency_data.getNumSamples ();

    juce::Image spectrogram {juce::Image::RGB, width, height, true};
    for (auto x = 0; x < width; ++x)
    {
        DrawSpectrogramLine (spectrogram, frequency_block.getSingleChannelBlock (x));
        spectrogram.moveImageSection (0, 0, 1, 0, width, height);
    }

    return spectrogram;
}
