#include "AudioGraphMetering.h"

auto read_peak = [] (const juce::dsp::AudioBlock<const float> & block)
{
    auto min_max = block.findMinAndMax ();
    return std::max (std::abs (min_max.getStart ()), std::abs (min_max.getEnd ()));
};

void AudioGraphMetering::UpdateChannelPeak (const juce::dsp::AudioBlock<const float> & audio_block)
{
    jassert (audio_block.getNumChannels () >= channels_.size ());

    for (auto channel_index = 0; channel_index < channels_.size (); ++channel_index)
    {
        auto channel_block = audio_block.getSingleChannelBlock (channel_index);
        auto peak = read_peak (channel_block);
        auto & channel = *channels_ [channel_index % channels_.size ()];

        if (channel.peak_value < peak)
            channel.peak_value.store (peak);

        if (peak >= 1.f)
            channel.is_clipping.store (true);
    }
}

void AudioGraphMetering::ResetClipping (size_t channel_index)
{
    auto & channel = channels_ [channel_index % channels_.size ()];
    channel->is_clipping.store (false);
}

void AudioGraphMetering::Prepare (size_t num_channels)
{
    channels_.resize (num_channels);
    for (auto & channel : channels_)
        channel = std::make_unique<ChannelMetering> ();
}

float AudioGraphMetering::GetChannelPeak (size_t channel_index)
{
    auto peak_value = channels_ [channel_index % channels_.size ()]->peak_value.load ();
    channels_ [channel_index % channels_.size ()]->peak_value = 0.0f;
    return peak_value;
}

bool AudioGraphMetering::GetChannelClipping (size_t channel_index)
{
    return channels_ [channel_index % channels_.size ()]->is_clipping;
}

int AudioGraphMetering::GetNumChannels ()
{
    return channels_.size ();
}
