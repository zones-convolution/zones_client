#pragma once
#include "juce_gui_extra/juce_gui_extra.h"
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

class ChannelBar : public juce::AnimatedAppComponent
{
public:
    ChannelBar ();
    void resized () override;
    void paint (juce::Graphics & g) override;

private:
public:
    void update () override;

private:
    DiscreteLevelBars discrete_level_bars_;
    MeterBar meter_bar_;
};
