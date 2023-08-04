#include "MeterComponent.h"

MeterComponent::MeterComponent ()
{
    for (auto & channel_bar : channel_bars_)
        addAndMakeVisible (channel_bar);

    addAndMakeVisible (discrete_level_labels_);
}

void MeterComponent::paint (juce::Graphics & g)
{
    Component::paint (g);
}

void MeterComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (discrete_level_labels_).withWidth (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);

    for (auto & channel_bar : channel_bars_)
    {
        layout.items.add (juce::FlexItem (channel_bar).withFlex (1.f));
        layout.items.add (juce::FlexItem ().withWidth (2.f));
    }

    layout.items.removeLast ();

    layout.performLayout (getLocalBounds ().toFloat ());
}
void MeterComponent::paintOverChildren (juce::Graphics & g)
{
    auto & left_bar = channel_bars_.front ();
    auto & right_bar = channel_bars_.back ();

    auto channels_border = juce::Rectangle<float> ()
                               .withWidth (right_bar.getRight () - left_bar.getX ())
                               .withHeight (getHeight ())
                               .withX (left_bar.getX ());

    juce::Path fake_rounded_corners;
    fake_rounded_corners.addRectangle (channels_border);
    fake_rounded_corners.setUsingNonZeroWinding (false);
    fake_rounded_corners.addRoundedRectangle (channels_border, LookAndFeel::kRounding);
    g.setColour (findColour (LookAndFeel::ColourIds::kPanel));
    g.fillPath (fake_rounded_corners);

    g.setColour (juce::Colours::white);
    g.drawRoundedRectangle (channels_border, LookAndFeel::kRounding, 2.f);
}
