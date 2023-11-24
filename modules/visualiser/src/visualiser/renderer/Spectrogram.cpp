#include "Spectrogram.h"

#include <tinycolormap.hpp>

static juce::Colour ConvertToJuceColour (const tinycolormap::Color & color)
{
    return juce::Colour::fromFloatRGBA (static_cast<float> (color.r ()),
                                        static_cast<float> (color.g ()),
                                        static_cast<float> (color.b ()),
                                        1.f);
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
            fft_data [fft_data_index],
            0.0f,
            juce::jmax (max_level.getEnd (), 1e-5f),
            0.0f,
            1.0f);
        level = std::clamp (level, 0.f, 1.f);

        auto colour = ConvertToJuceColour (
                tinycolormap::GetColor (level, tinycolormap::ColormapType::Viridis))
            .withAlpha (level);

        spectrogram.setPixelAt (right_hand_edge, y, colour);
    }
}

immer::box<juce::AudioBuffer<float>>
Spectrogram::PerformFFT (const juce::dsp::AudioBlock<const float> & audio_block)
{
    static constexpr auto kFFTOrder = 10;
    static constexpr auto kFFTSize = 1 << kFFTOrder;
    std::array<float, kFFTSize * 2> fft_data{};

    juce::dsp::FFT fft{kFFTOrder};
    juce::dsp::WindowingFunction<float> window{kFFTSize,
                                               juce::dsp::WindowingFunction<float>::hann};

    auto num_samples = static_cast<int> (audio_block.getNumSamples ());
    auto hop_length = kFFTSize / 2;
    auto num_hops = (num_samples / hop_length) + 1;

    juce::AudioBuffer<float> fft_buffer;
    fft_buffer.setSize (num_hops, fft_data.size ());
    fft_buffer.clear ();

    auto start_sample_index = 0;
    while (start_sample_index < num_samples)
    {
        auto hop_index = start_sample_index / hop_length;
        auto block_size = (start_sample_index + kFFTSize > num_samples)
                              ? num_samples - start_sample_index
                              : kFFTSize;
        auto sub_block = audio_block.getSubBlock (start_sample_index, block_size);

        juce::FloatVectorOperations::fill (fft_data.data (), 0.f, fft_data.size ());
        for (auto channel = 0; channel < audio_block.getNumChannels (); ++channel)
        {
            auto read_pointer = sub_block.getChannelPointer (channel);
            juce::FloatVectorOperations::add (fft_data.data (), read_pointer, kFFTSize);
        }

        window.multiplyWithWindowingTable (fft_data.data (), kFFTSize);
        fft.performFrequencyOnlyForwardTransform (fft_data.data (), true);

        auto fft_channel = fft_buffer.getWritePointer (hop_index);
        juce::FloatVectorOperations::copy (fft_channel, fft_data.data (), fft_data.size ());
        start_sample_index += hop_length;
    }

    return {std::move (fft_buffer)};
}

juce::Image Spectrogram::CreateSpectrogram (const juce::dsp::AudioBlock<const float> & audio_block)
{
    auto fft_buffer = PerformFFT (audio_block);
    auto num_hops = fft_buffer->getNumChannels ();
    auto fft_size = fft_buffer->getNumSamples ();

    juce::Image spectrogram{juce::Image::RGB, num_hops, fft_size, true};
    for (auto fft_data_index = 0; fft_data_index < num_hops; ++fft_data_index)
        DrawSpectrogramLine (spectrogram, fft_buffer->getReadPointer (fft_data_index), fft_size);
    return spectrogram;
}
