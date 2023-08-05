#pragma once
#include "ChannelBar.h"
#include "ClippingIndicatorsComponent.h"
#include "DiscreteLevelLabels.h"
#include "audio_engine/AudioGraphMetering.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::AnimatedAppComponent
{
public:
    explicit MeterComponent (AudioGraphMetering & audio_graph_metering);
    void resized () override;
    void paintOverChildren (juce::Graphics & g) override;
    void paint (juce::Graphics & g) override;

    void update () override;

private:
    AudioGraphMetering & audio_graph_metering_;
    std::array<ChannelBar, 2> channel_bars_;
    DiscreteLevelBars discrete_level_bars_;
    DiscreteLevelLabels discrete_level_labels_;
    ClippingIndicatorsComponent clipping_indicators_component_;
    juce::Rectangle<float> GetChannelBounds ();
};
