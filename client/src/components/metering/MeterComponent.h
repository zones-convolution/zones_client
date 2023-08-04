#pragma once
#include "ChannelBar.h"
#include "DiscreteLevelLabels.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::Component
{
public:
    MeterComponent ();
    void paint (juce::Graphics & g) override;
    void resized () override;
    void paintOverChildren (juce::Graphics & g) override;

private:
    std::array<ChannelBar, 4> channel_bars_;

    DiscreteLevelLabels discrete_level_labels_;
};
