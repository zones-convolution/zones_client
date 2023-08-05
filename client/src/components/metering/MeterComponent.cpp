#include "MeterComponent.h"

MeterComponent::MeterComponent (AudioGraphMetering & audio_graph_metering)
    : audio_graph_metering_ (audio_graph_metering)
{
    for (auto & channel_bar : channel_bars_)
        addAndMakeVisible (channel_bar);

    addAndMakeVisible (discrete_level_bars_);
    addAndMakeVisible (discrete_level_labels_);

    setSynchroniseToVBlank (true);
}

static constexpr auto kSpacing = 2.f;
static constexpr auto kMargin = 6.f;

void MeterComponent::resized ()
{
    juce::FlexBox bar_layout;
    bar_layout.flexDirection = juce::FlexBox::Direction::row;
    for (auto & channel_bar : channel_bars_)
    {
        bar_layout.items.add (juce::FlexItem (channel_bar).withFlex (1.f));
        bar_layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    }
    bar_layout.items.removeLast ();

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (discrete_level_labels_).withWidth (26.f));
    layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    layout.items.add (juce::FlexItem (discrete_level_bars_)
                          .withWidth (3 * kSpacing)
                          .withMargin (juce::FlexItem::Margin (kMargin, 0, kMargin, 0)));
    layout.items.add (
        juce::FlexItem (bar_layout).withFlex (1.f).withMargin (juce::FlexItem::Margin (kMargin)));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void MeterComponent::paint (juce::Graphics & g)
{
    auto channels_bounds = GetChannelBounds ().expanded (kMargin);
    g.setColour (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillRect (channels_bounds);
}

void MeterComponent::update ()
{
    for (auto channel_index = 0; channel_index < channel_bars_.size (); ++channel_index)
    {
        auto peak = audio_graph_metering_.GetChannelPeak (channel_index);
        channel_bars_ [channel_index].SetTarget (peak);
    }
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
    auto left_bar_bounds = channel_bars_.front ().getBounds ().toFloat ();
    auto right_bar_bounds = channel_bars_.back ().getBounds ().toFloat ();
    auto channels_bounds = left_bar_bounds.getUnion (right_bar_bounds);
    return channels_bounds;
}
