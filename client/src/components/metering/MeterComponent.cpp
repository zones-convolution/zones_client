#include "MeterComponent.h"

#include <zones_look_and_feel/LookAndFeel.h>

MeterComponent::MeterComponent (AudioGraphMetering & input_graph_metering,
                                AudioGraphMetering & output_graph_metering)
    : input_graph_metering_ (input_graph_metering)
    , output_graph_metering_ (output_graph_metering)
{
    addAndMakeVisible (discrete_level_bars_);
    addAndMakeVisible (discrete_level_labels_);
    addAndMakeVisible (clipping_indicators_component_);

    setSynchroniseToVBlank (true);
}

static constexpr auto kSpacing = 2.f;
static constexpr auto kMargin = 6.f;
static constexpr auto kDiscreteLevelLabelWidth = 26.f;
static constexpr auto kClippingIndicatorHeight = 6.f;

void MeterComponent::SetChannelConfiguration (size_t num_input_channels, size_t num_output_channels)
{
    for (auto & channel : input_channels_)
        removeChildComponent (&channel->bar);

    for (auto & channel : output_channels_)
        removeChildComponent (&channel->bar);

    input_channels_.clear ();
    output_channels_.clear ();

    for (auto channel_index = 0; channel_index < num_input_channels; ++channel_index)
    {
        input_channels_.push_back (std::make_unique<ChannelMeter> ());
        addAndMakeVisible (input_channels_ [channel_index]->bar);
    }

    for (auto channel_index = 0; channel_index < num_output_channels; ++channel_index)
    {
        output_channels_.push_back (std::make_unique<ChannelMeter> ());
        addAndMakeVisible (input_channels_ [channel_index]->bar);
    }
}

void MeterComponent::resized ()
{
    auto bar_layout = CreateBarLayout ();
    auto side_layout = CreateSideLayout ();
    side_layout.items.add (
        juce::FlexItem (bar_layout).withFlex (1.f).withMargin (juce::FlexItem::Margin (kMargin)));

    auto clipping_indicator_layout = CreateClippingIndicatorLayout ();

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (
        juce::FlexItem (clipping_indicator_layout).withHeight (kClippingIndicatorHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (side_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

juce::FlexBox MeterComponent::CreateSideLayout ()
{
    juce::FlexBox side_layout;
    side_layout.flexDirection = juce::FlexBox::Direction::row;

    side_layout.items.add (
        juce::FlexItem (discrete_level_labels_).withWidth (kDiscreteLevelLabelWidth));
    side_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    side_layout.items.add (juce::FlexItem (discrete_level_bars_)
                               .withWidth (3 * kSpacing)
                               .withMargin (juce::FlexItem::Margin (kMargin, 0, kMargin, 0)));
    return side_layout;
}

juce::FlexBox MeterComponent::CreateBarLayout ()
{
    juce::FlexBox bar_layout;
    bar_layout.flexDirection = juce::FlexBox::Direction::row;

    auto layout_channels = [&bar_layout] (std::vector<std::unique_ptr<ChannelMeter>> & channels)
    {
        for (auto & channel : channels)
        {
            bar_layout.items.add (juce::FlexItem (channel->bar).withFlex (1.f));
            bar_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
        }

        bar_layout.items.removeLast ();
    };

    layout_channels (input_channels_);
    bar_layout.items.add (LookAndFeel::kFlexSpacer);
    layout_channels (output_channels_);

    return bar_layout;
}

juce::FlexBox MeterComponent::CreateClippingIndicatorLayout ()
{
    juce::FlexBox indicator_layout;
    indicator_layout.flexDirection = juce::FlexBox::Direction::row;
    indicator_layout.items.add (
        juce::FlexItem ().withWidth (kDiscreteLevelLabelWidth + 3 * kSpacing));
    indicator_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    indicator_layout.items.add (juce::FlexItem (clipping_indicators_component_)
                                    .withFlex (1.f)
                                    .withMargin (juce::FlexItem::Margin (0, kMargin, 0, kMargin)));
    return indicator_layout;
}

void MeterComponent::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    auto channels_bounds = GetChannelBounds ().expanded (kMargin);
    g.setColour (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillRect (channels_bounds);
}

static float SmoothedValue (float value_to_smooth, float target, float smoothing_constant)
{
    auto delta = 1.0f / smoothing_constant;
    auto step = (target - value_to_smooth) * delta;
    return value_to_smooth + step;
}

void MeterComponent::update ()
{
    auto update_metering = [&] (AudioGraphMetering & audio_graph_metering,
                                std::vector<std::unique_ptr<ChannelMeter>> & channels)
    {
        for (auto channel_index = 0; channel_index < channels.size (); ++channel_index)
        {
            auto peak = audio_graph_metering.GetChannelPeak (channel_index);
            static constexpr auto kSmoothingConstant = 2.f;

            auto & channel = channels [channel_index];
            auto & smoothed_target = channel->smoothed_value;
            smoothed_target =
                peak < smoothed_target
                    ? SmoothedValue (smoothed_target,
                                     peak,
                                     static_cast<float> (getMillisecondsSinceLastUpdate ()) *
                                         kSmoothingConstant)
                    : peak;

            auto & peak_target = channel->smoothed_peak;
            auto & peak_fade_timer = channel->peak_fade_timer;

            if (peak >= peak_target)
            {
                peak_target = peak;
                peak_fade_timer = 0;
            }
            else
            {
                peak_fade_timer += getMillisecondsSinceLastUpdate ();
                static constexpr auto kPeakFadeDurationMs = 2000;
                if (peak_fade_timer >= kPeakFadeDurationMs)
                    peak_target =
                        SmoothedValue (peak_target,
                                       0,
                                       static_cast<float> (getMillisecondsSinceLastUpdate ()) *
                                           kSmoothingConstant * 4);
            }

            channel->bar.SetTarget (smoothed_target, peak_target);
            if (peak >= 1.f)
                clipping_indicators_component_.SetIndicator (channel_index, true);
        }
    };

    update_metering (input_graph_metering_, input_channels_);
    update_metering (output_graph_metering_, output_channels_);
}

void MeterComponent::paintOverChildren (juce::Graphics & g)
{
    auto channels_bounds = GetChannelBounds ().expanded (kMargin);

    juce::Path rounded_path;
    static constexpr auto kLineThickness = 1.f;
    auto kHalfLineThickness = kLineThickness / 2.f;

    rounded_path.addRoundedRectangle (channels_bounds.getX () + kHalfLineThickness,
                                      channels_bounds.getY () + kHalfLineThickness,
                                      channels_bounds.getWidth () - kLineThickness,
                                      channels_bounds.getHeight () - kLineThickness,
                                      LookAndFeel::kRounding);

    juce::Path fake_rounded_corners = rounded_path;
    fake_rounded_corners.addRectangle (channels_bounds);
    fake_rounded_corners.setUsingNonZeroWinding (false);
    g.setColour (findColour (LookAndFeel::kPanel));
    g.fillPath (fake_rounded_corners);

    g.setColour (juce::Colours::white);
    g.strokePath (rounded_path, juce::PathStrokeType (kLineThickness));
}

juce::Rectangle<float> MeterComponent::GetChannelBounds ()
{
    auto left_bar_bounds = input_channels_.front ()->bar.getBounds ().toFloat ();
    auto right_bar_bounds = output_channels_.back ()->bar.getBounds ().toFloat ();
    auto channels_bounds = left_bar_bounds.getUnion (right_bar_bounds);
    return channels_bounds;
}
