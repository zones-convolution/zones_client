#include "Buttons.h"

Buttons::Buttons ()
{
    buttons_label_.setText ("Buttons", juce::dontSendNotification);
    buttons_label_.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (buttons_label_);
    addAndMakeVisible (button_primary_);
    addAndMakeVisible (button_secondary_);

    button_disabled_.setEnabled (false);
    addAndMakeVisible (button_disabled_);

    addAndMakeVisible (icon_text_button_);

    icon_text_button_.onClick = [&] ()
    { icon_text_button_.setButtonText (juce::Uuid ().toString ()); };
}

void Buttons::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (buttons_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (button_primary_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (button_secondary_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (button_disabled_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (icon_text_button_).withFlex (1.f));

    layout.performLayout (getLocalBounds ());
}