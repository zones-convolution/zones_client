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

private:
    AudioGraphMetering & input_metering_;
    AudioGraphMetering & output_metering_;
};
