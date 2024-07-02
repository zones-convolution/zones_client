#pragma once

#include "audio_engine/AudioGraphMetering.h"

#include <juce_gui_extra/juce_gui_extra.h>

class MeteringRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    MeteringRelay (AudioGraphMetering & input_metering, AudioGraphMetering & output_metering);
    ~MeteringRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

    struct ChannelMetering
    {
        bool is_clipping;
        float peak_value;
    };

    struct Metering
    {
        std::vector<ChannelMetering> input_channels;
        std::vector<ChannelMetering> output_channels;
    };

private:
    AudioGraphMetering & input_metering_;
    AudioGraphMetering & output_metering_;
};
