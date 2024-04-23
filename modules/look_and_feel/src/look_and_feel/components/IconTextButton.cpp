#include "IconTextButton.h"

#include "look_and_feel/LookAndFeel.h"

IconTextButton::IconTextButton ()
    : IconTextButton (juce::String ())
{
}

IconTextButton::IconTextButton (const juce::String & name)
    : Button (name)
{
    addAndMakeVisible (label_);
    label_.setText (name, juce::dontSendNotification);
    label_.setInterceptsMouseClicks (false, false);

    addAndMakeVisible (icon_);
}

IconTextButton::IconTextButton (const juce::String & name, const juce::String & icon)
    : IconTextButton (name)
{
    SetIcon (icon);
}

IconTextButton::IconTextButton (const juce::String & name,
                                const juce::String & icon,
                                const juce::String & tool_tip)
    : IconTextButton (name, icon)
{
    setTooltip (tool_tip);
}

void IconTextButton::paintButton (juce::Graphics & g,
                                  bool shouldDrawButtonAsHighlighted,
                                  bool shouldDrawButtonAsDown)
{
    auto & look_and_feel = getLookAndFeel ();

    if (this->getButtonText () != label_.getText ())
    {
        label_.setText (this->getButtonText (), juce::dontSendNotification);
        resized ();
    }

    look_and_feel.drawButtonBackground (g,
                                        *this,
                                        findColour (getToggleState ()
                                                        ? juce::TextButton::buttonOnColourId
                                                        : juce::TextButton::buttonColourId),
                                        shouldDrawButtonAsHighlighted,
                                        shouldDrawButtonAsDown);
}

void IconTextButton::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (LookAndFeel::LabelFlexItem (label_));
    layout.items.add (LookAndFeel::MediumIconFlexItem (icon_));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void IconTextButton::SetIcon (const juce::String & icon)
{
    icon_.SetIcon (icon);
}
