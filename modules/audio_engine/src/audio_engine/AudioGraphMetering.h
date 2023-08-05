#pragma once
#include <atomic>
#include <juce_dsp/juce_dsp.h>
#include <vector>

class AudioGraphMetering
{
public:
    void UpdateChannelPeak (const juce::dsp::AudioBlock<const float> & audio_block);
    void ResetClipping ();
    void Prepare (size_t num_channels);
    float GetChannelPeak (size_t channel_index);
    bool GetChannelClipping (size_t channel_index);

private:
    std::vector<std::unique_ptr<std::atomic<bool>>> channel_clipping_;
    std::vector<std::unique_ptr<std::atomic<float>>> channel_peaks_;
};
