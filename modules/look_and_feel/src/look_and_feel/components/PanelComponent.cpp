#include "PanelComponent.h"

PanelComponent::PanelComponent (juce::Component & child, bool apply_content_rounding)
    : apply_content_rounding_ (apply_content_rounding)
    , child_ (child)
{
    addAndMakeVisible (child_);
}

PanelComponent::PanelComponent (juce::Component & child,
                                ColourPair horizontal_gradient_colours,
                                bool apply_content_rounding)
    : PanelComponent (child, apply_content_rounding)
{
    background_gradient_ = horizontal_gradient_colours;
}

PanelComponent::~PanelComponent ()
{
    removeChildComponent (&child_);
}

void PanelComponent::paint (juce::Graphics & g)
{
    if (background_gradient_.has_value ())
        g.setGradientFill (juce::ColourGradient::horizontal (
            background_gradient_->first, background_gradient_->second, getLocalBounds ()));
    else
        g.setColour (findColour (LookAndFeel::ColourIds::kPanel));

    g.fillRoundedRectangle (getLocalBounds ().toFloat (), LookAndFeel::kRounding);
}

void PanelComponent::resized ()
{
    juce::FlexBox box;
    box.justifyContent = juce::FlexBox::JustifyContent::center;

    box.items.add (juce::FlexItem (child_).withFlex (1.f));
    box.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void PanelComponent::paintOverChildren (juce::Graphics & g)
{
    if (! apply_content_rounding_)
        return juce::Component::paintOverChildren (g);

    juce::Path fake_rounded_corners;
    auto bounds = getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding);
    fake_rounded_corners.addRectangle (bounds);
    fake_rounded_corners.setUsingNonZeroWinding (false);
    fake_rounded_corners.addRoundedRectangle (bounds, LookAndFeel::kRounding);
    g.setColour (findColour (LookAndFeel::ColourIds::kPanel));
    g.fillPath (fake_rounded_corners);
}