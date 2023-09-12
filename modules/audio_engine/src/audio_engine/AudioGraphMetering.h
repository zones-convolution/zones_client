#pragma once
#include <atomic>
#include <juce_dsp/juce_dsp.h>
#include <vector>

class AudioGraphMetering
{
public:
    void UpdateChannelPeak (const juce::dsp::AudioBlock<const float> & audio_block);
    void ResetClipping (size_t channel_index);
    void Prepare (size_t num_channels);
    float GetChannelPeak (size_t channel_index);
    bool GetChannelClipping (size_t channel_index);

private:
    struct ChannelMetering
    {
        std::atomic<bool> is_clipping;
        std::atomic<float> peak_value;
    };

    std::vector<std::unique_ptr<ChannelMetering>> channels_;
};
