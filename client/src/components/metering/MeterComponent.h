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
    explicit MeterComponent (AudioGraphMetering & input_graph_metering,
                             AudioGraphMetering & output_graph_metering);

    void resized () override;
    void paintOverChildren (juce::Graphics & g) override;
    void paint (juce::Graphics & g) override;
    void update () override;

    void SetChannelConfiguration (size_t num_input_channels, size_t num_output_channels);

private:
    juce::FlexBox CreateClippingIndicatorLayout ();
    juce::FlexBox CreateBarLayout ();
    juce::FlexBox CreateSideLayout ();

    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;

    struct ChannelMeter
    {
        ChannelBar bar;
        float smoothed_value = 0.f;
        float smoothed_peak = 0.f;
        int peak_fade_timer = 0;
    };

    std::vector<std::unique_ptr<ChannelMeter>> input_channels_;
    std::vector<std::unique_ptr<ChannelMeter>> output_channels_;

    DiscreteLevelBars discrete_level_bars_;
    DiscreteLevelLabels discrete_level_labels_;
    ClippingIndicatorsComponent clipping_indicators_component_;
    juce::Rectangle<float> GetChannelBounds ();
};
