#include "MeterComponent.h"

#include "look_and_feel/LookAndFeel.h"

MeterComponent::MeterComponent ()
{
    addAndMakeVisible (discrete_level_bars_);
    addAndMakeVisible (discrete_level_labels_);

    setSynchroniseToVBlank (true);
}

static constexpr auto kSpacing = 2.f;
static constexpr auto kMargin = 6.f;
static constexpr auto kDiscreteLevelLabelWidth = 26.f;
static constexpr auto kClippingIndicatorHeight = 6.f;

void MeterComponent::SetConfiguration (MeterComponent::ChannelConfiguration configuration)
{
    for (auto & channel_group : channel_groups_)
    {
        for (auto & channel : channel_group)
        {
            removeChildComponent (&channel->bar);
            removeChildComponent (&channel->clipping_indicator);
        }
    }

    channel_groups_.clear ();
    channel_groups_.resize (configuration.size ());

    for (auto group_index = 0; group_index < configuration.size (); ++group_index)
    {
        auto & group_configuration = configuration [group_index];
        auto & channel_group = channel_groups_ [group_index];

        for (auto & channel_configuration : group_configuration)
        {
            auto channel = std::make_unique<ChannelMeter> ();
            channel->delegate = channel_configuration;
            addAndMakeVisible (channel->bar);
            addAndMakeVisible (channel->clipping_indicator);

            auto & delegate = channel->delegate;
            channel->clipping_indicator.onClick = [&delegate] () { delegate.reset_clipping (); };

            channel_group.push_back (std::move (channel));
        }
    }
}

void MeterComponent::resized ()
{
    juce::Component bar_layout;

    auto side_layout = CreateSideLayout ();
    side_layout.items.add (
        juce::FlexItem (bar_layout).withFlex (1.f).withMargin (juce::FlexItem::Margin (kMargin)));

    auto clipping_side_layout = CreateClippingIndicatorLayout ();

    juce::Component clipping_indicator_layout;
    clipping_side_layout.items.add (
        juce::FlexItem (clipping_indicator_layout)
            .withFlex (1.f)
            .withMargin (juce::FlexItem::Margin (0, kMargin, 0, kMargin)));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (juce::FlexItem (clipping_side_layout).withHeight (kClippingIndicatorHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (side_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
    LayoutBars (bar_layout.getBounds ().toFloat ());
    LayoutClippingIndicators (clipping_indicator_layout.getBounds ().toFloat ());
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

juce::FlexBox MeterComponent::CreateClippingIndicatorLayout ()
{
    juce::FlexBox indicator_layout;
    indicator_layout.flexDirection = juce::FlexBox::Direction::row;
    indicator_layout.items.add (
        juce::FlexItem ().withWidth (kDiscreteLevelLabelWidth + 3 * kSpacing));
    indicator_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    return indicator_layout;
}

void MeterComponent::LayoutBars (const juce::Rectangle<float> & bar_bounds)
{
    juce::FlexBox bar_layout;
    bar_layout.flexDirection = juce::FlexBox::Direction::row;

    /* This sequence is a bit fragile however is needed to ensure the width of each bar is exactly
     * the same, something FlexBox doesn't always guarantee. */

    int num_bars = 0;
    auto group_spacing = (channel_groups_.size () - 1.0f) * (2.0f * kSpacing);

    for (auto & channel_group : channel_groups_)
        num_bars += channel_group.size ();

    auto spaced_bounds =
        bar_bounds.getWidth () - (static_cast<float> (num_bars) * kSpacing) - group_spacing;
    auto fixed_bar_width = std::ceil (spaced_bounds / static_cast<float> (num_bars));

    auto layout_channel_group =
        [&bar_layout, &fixed_bar_width] (std::vector<std::unique_ptr<ChannelMeter>> & channel_group)
    {
        for (auto & channel : channel_group)
        {
            bar_layout.items.add (juce::FlexItem (channel->bar).withWidth (fixed_bar_width));
            bar_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
        }

        bar_layout.items.removeLast ();
    };

    for (auto & channel_group : channel_groups_)
    {
        layout_channel_group (channel_group);
        bar_layout.items.add (juce::FlexItem ().withFlex (1.f));
    }

    bar_layout.items.removeLast ();
    bar_layout.performLayout (bar_bounds);
}

void MeterComponent::LayoutClippingIndicators (const juce::Rectangle<float> & clipping_bounds)
{
    for (auto & channel_group : channel_groups_)
    {
        for (auto & channel : channel_group)
        {
            auto bar_bounds = channel->bar.getBounds ().toFloat ();
            auto indicator_bounds = bar_bounds.withHeight (clipping_bounds.getHeight ())
                                        .withY (clipping_bounds.getY ())
                                        .toNearestInt ();
            channel->clipping_indicator.setBounds (indicator_bounds);
        }
    }
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
    auto update_metering = [&] (std::vector<std::unique_ptr<ChannelMeter>> & channel_group)
    {
        for (auto channel_index = 0; channel_index < channel_group.size (); ++channel_index)
        {
            auto & channel = channel_group [channel_index];

            auto peak = channel->delegate.get_peak ();
            auto is_clipping = channel->delegate.is_clipping ();
            static constexpr auto kSmoothingConstant = 2.f;

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
            channel->clipping_indicator.SetFill (is_clipping);
        }
    };

    for (auto & channel_group : channel_groups_)
        update_metering (channel_group);
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
    auto & left_most_channel = channel_groups_.front ().front ();
    auto left_bar_bounds = left_most_channel->bar.getBounds ().toFloat ();

    auto & right_most_channel = channel_groups_.back ().back ();
    auto right_bar_bounds = right_most_channel->bar.getBounds ().toFloat ();

    auto channels_bounds = left_bar_bounds.getUnion (right_bar_bounds);
    return channels_bounds;
}