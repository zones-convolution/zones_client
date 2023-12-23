#pragma once
#include "ChannelBar.h"
#include "ClippingIndicator.h"
#include "DiscreteLevelLabels.h"
#include "LabelHeightContainer.h"
#include "audio_engine/AudioGraphMetering.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <immer/flex_vector.hpp>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::AnimatedAppComponent
{
public:
    struct ChannelMeterDelegate
    {
        std::function<float ()> get_peak;
        std::function<bool ()> is_clipping;
        std::function<void ()> reset_clipping;
    };

    MeterComponent ();

    void resized () override;
    void paintOverChildren (juce::Graphics & g) override;
    void paint (juce::Graphics & g) override;
    void update () override;

    using ChannelConfiguration = immer::flex_vector<immer::flex_vector<ChannelMeterDelegate>>;
    void SetConfiguration (ChannelConfiguration configuration);

private:
    juce::FlexBox CreateClippingIndicatorLayout ();
    void LayoutBars (const juce::Rectangle<float> & bar_bounds);
    void LayoutClippingIndicators (const juce::Rectangle<float> & clipping_bounds);
    juce::FlexBox CreateSideLayout ();

    struct ChannelMeter
    {
        ChannelBar bar;
        ClippingIndicator clipping_indicator;
        ChannelMeterDelegate delegate;
        float smoothed_value = 0.f;
        float smoothed_peak = 0.f;
        int peak_fade_timer = 0;
    };

    std::vector<std::vector<std::unique_ptr<ChannelMeter>>> channel_groups_;

    std::vector<float> label_values_ {-3, -6, -9, -12, -15, -18, -21, -30, -60};
    LabelHeightContainer label_height_container_;
    DiscreteLevelBars discrete_level_bars_;
    DiscreteLevelLabels discrete_level_labels_;
    juce::Rectangle<float> GetChannelBounds ();
};
