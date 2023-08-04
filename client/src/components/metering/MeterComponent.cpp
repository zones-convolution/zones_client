#include "MeterComponent.h"

MeterComponent::MeterComponent ()
{
    for (auto & channel_bar : channel_bars_)
        addAndMakeVisible (channel_bar);

    addAndMakeVisible (discrete_level_bars_);
    addAndMakeVisible (discrete_level_labels_);
}

void MeterComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    static constexpr auto kSpacing = 2.f;
    layout.items.add (juce::FlexItem (discrete_level_labels_).withWidth (26.f));
    layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    layout.items.add (juce::FlexItem (discrete_level_bars_).withWidth (3 * kSpacing));

    for (auto & channel_bar : channel_bars_)
    {
        layout.items.add (juce::FlexItem (channel_bar).withFlex (1.f));
        layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    }

    layout.items.removeLast ();

    layout.performLayout (getLocalBounds ().toFloat ());
}

void MeterComponent::paintOverChildren (juce::Graphics & g)
{
    auto left_bar_bounds = channel_bars_.front ().getBounds ().toFloat ();
    auto right_bar_bounds = channel_bars_.back ().getBounds ().toFloat ();
    auto channels_bounds = left_bar_bounds.getUnion (right_bar_bounds);

    juce::Path rounded_path;
    static constexpr auto kLineThickness = 2.f;
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
