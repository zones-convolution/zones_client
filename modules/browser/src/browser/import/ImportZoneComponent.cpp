#include "ImportZoneComponent.h"

#include "look_and_feel/LookAndFeel.h"
#include "preferences/Preferences.h"

ImportZoneComponent::ImportZoneComponent ()
{
    auto placeholder_colour = juce::Colours::grey.withAlpha (0.6f);
    title_input_.setTextToShowWhenEmpty ("Ir Name", placeholder_colour);
    title_input_.setMultiLine (false, false);
    addAndMakeVisible (title_input_);

    description_input_.setTextToShowWhenEmpty ("Ir Description", placeholder_colour);
    description_input_.setMultiLine (true, true);
    addAndMakeVisible (description_input_);

    Preferences preferences;
    preferences.Load ();

    auto user_paths = preferences.user_paths;
    for (auto i = 0; i < user_paths.size (); ++i)
        user_path_picker_.addItem (user_paths [i].string (), i + 1);
    user_path_picker_.setSelectedItemIndex (0);
    addAndMakeVisible (user_path_picker_);
}

void ImportZoneComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (title_input_).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (
        juce::FlexItem (description_input_).withHeight (LookAndFeel::kButtonHeight * 4));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (user_path_picker_).withHeight (LookAndFeel::kButtonHeight));
    layout.items.add (LookAndFeel::kFlexSpacer);

    layout.performLayout (getLocalBounds ().toFloat ());
}

std::string ImportZoneComponent::GetDescription () const
{
    return description_input_.getText ().toStdString ();
}

std::string ImportZoneComponent::GetTitle () const
{
    return title_input_.getText ().toStdString ();
}

std::string ImportZoneComponent::GetUserPath () const
{
    return user_path_picker_.getText ().toStdString ();
}
