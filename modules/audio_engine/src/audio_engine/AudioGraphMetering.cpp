#include "AudioGraphMetering.h"

auto read_peak = [] (const juce::dsp::AudioBlock<const float> & block)
{
    auto min_max = block.findMinAndMax ();
    return std::max (std::abs (min_max.getStart ()), std::abs (min_max.getEnd ()));
};

void AudioGraphMetering::UpdateChannelPeak (const juce::dsp::AudioBlock<const float> & audio_block)
{
    for (auto channel_index = 0; channel_index < audio_block.getNumChannels (); ++channel_index)
    {
        auto channel_block = audio_block.getSingleChannelBlock (channel_index);
        auto peak = read_peak (channel_block);

        channel_peaks_ [channel_index]->store (peak);
        if (peak >= 1.f)
            channel_clipping_ [channel_index]->store (true);
    }
}

void AudioGraphMetering::ResetClipping ()
{
    for (auto & channel_clipping : channel_clipping_)
        channel_clipping->store (false);
}

void AudioGraphMetering::Prepare (size_t num_channels)
{
    channel_peaks_.resize (num_channels);
    channel_clipping_.resize (num_channels);

    for (auto & channel_peak : channel_peaks_)
        channel_peak = std::make_unique<std::atomic<float>> ();

    for (auto & channel_clipping : channel_clipping_)
        channel_clipping = std::make_unique<std::atomic<bool>> ();
}

float AudioGraphMetering::GetChannelPeak (size_t channel_index)
{
    return *channel_peaks_ [channel_index];
}

bool AudioGraphMetering::GetChannelClipping (size_t channel_index)
{
    return *channel_clipping_ [channel_index];
}
