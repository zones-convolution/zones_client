#pragma once
#include "juce_gui_extra/juce_gui_extra.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
class MeterBar
    : public juce::Component
    , public juce::Timer
{
public:
    void paint (juce::Graphics & g) override;
    void SetFill (float fill);

private:
    void timerCallback () override;

    static const std::pair<juce::Colour, juce::Colour> kMeterGradientColours;
    float fill_ = 0.8f;
    float highest_value_ = 0.0f;
};

class DiscreteLevelBars : public juce::Component
{
public:
    void paint (juce::Graphics & g) override;
};

class ChannelBar : public juce::Component
{
public:
    ChannelBar ();
    void resized () override;
    void paint (juce::Graphics & g) override;
    void SetTarget (float target);

private:
    DiscreteLevelBars discrete_level_bars_;
    MeterBar meter_bar_;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothed_level_ {0.f};
};
