#pragma once
#include "LabelContainer.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterBar : public juce::Component
{
public:
    void paint (juce::Graphics & g) override;
    void SetFill (float fill);
    void SetPeak (float peak);

private:
    static const std::pair<juce::Colour, juce::Colour> kMeterGradientColours;
    float fill_ = 0.8f;
    float peak_target_value_ = 0.0f;
};

class DiscreteLevelBars : public juce::Component
{
public:
    DiscreteLevelBars (LabelContainer & label_container);
    void paint (juce::Graphics & g) override;

private:
    LabelContainer & label_container_;
};

class ChannelBar : public juce::Component
{
public:
    ChannelBar (LabelContainer & label_height_container);
    void resized () override;
    void paint (juce::Graphics & g) override;
    void SetTarget (float target, float peak);

private:
    DiscreteLevelBars discrete_level_bars_;
    MeterBar meter_bar_;

    float smoothed_level_ = 0.f;
};
