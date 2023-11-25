#include "Spectrogram.h"

#include <tinycolormap.hpp>

static juce::Colour ConvertToJuceColour (const tinycolormap::Color & color)
{
    return juce::Colour::fromFloatRGBA (color.r (), color.g (), color.b (), 1.f);
}

static void DrawSpectrogramLine (juce::Image & spectrogram, const float * fft_data, int fft_size)
{
    auto right_hand_edge = spectrogram.getWidth () - 1;
    auto image_height = spectrogram.getHeight ();
    spectrogram.moveImageSection (0, 0, 1, 0, right_hand_edge, image_height);
    auto max_level = juce::FloatVectorOperations::findMinAndMax (fft_data, fft_size / 2);
    for (auto y = 1; y < image_height; ++y)
    {
        auto y_prop = 1.0f - ((float) y / (float) image_height);
        auto skewed_proportion_y = std::exp (std::log (0.2f * y_prop));
        auto fft_data_index =
            (size_t) juce::jlimit (0, fft_size / 2, (int) (skewed_proportion_y * fft_size / 2));
        auto level = juce::jmap (
            fft_data [fft_data_index], 0.0f, juce::jmax (max_level.getEnd (), 1e-5f), 0.0f, 1.0f);
        level = std::clamp (level, 0.f, 1.f);

        auto colour = ConvertToJuceColour (
                          tinycolormap::GetColor (level, tinycolormap::ColormapType::Viridis))
                          .withAlpha (level);

        spectrogram.setPixelAt (right_hand_edge, y, colour);
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
                                             static_cast<int> (fft_size)};
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
        fft.performFrequencyOnlyForwardTransform (fft_block.getChannelPointer (0));
        frequency_block.getSingleChannelBlock (hop_index).copyFrom (
            fft_block.getSubBlock (0, fft_size));
    }

    return {std::move (frequency_data)};
}

juce::Image Spectrogram::CreateSpectrogram (const juce::dsp::AudioBlock<const float> & audio_block)
{
    static constexpr auto kFFTOrder = 10u;

    auto frequency_data = PerformFFT (audio_block, kFFTOrder);
    juce::dsp::AudioBlock<float> frequency_block {frequency_data};

    auto num_hops = frequency_data.getNumChannels ();
    auto fft_size = frequency_data.getNumSamples ();

    juce::Image spectrogram {juce::Image::RGB, num_hops, fft_size, true};
    for (auto fft_data_index = 0; fft_data_index < num_hops; ++fft_data_index)
        DrawSpectrogramLine (spectrogram, frequency_data.getReadPointer (fft_data_index), fft_size);
    return spectrogram;
}
