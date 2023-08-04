#pragma once
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class MeterBar : public juce::Component
{
public:
    void paint (juce::Graphics & g) override;
    void SetFill (float fill);

private:
    static const std::pair<juce::Colour, juce::Colour> kMeterGradientColours;
    float fill_ = 0.8f;
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

private:
    DiscreteLevelBars discrete_level_bars_;
    MeterBar meter_bar_;
};
