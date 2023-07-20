#include "Combos.h"

Combos::Combos ()
{
    combos_label_.setText ("Combos", juce::dontSendNotification);
    combos_label_.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (combos_label_);

    combo_box_small_.addItemList ({"Item 1", "Item 2", "Item 3"}, 1);
    combo_box_small_.setSelectedId (1, juce::dontSendNotification);
    addAndMakeVisible (combo_box_small_);

    combo_box_large_.addItemList ({"Item 1", "Item 2", "Item 3"}, 1);
    combo_box_large_.setSelectedId (1, juce::dontSendNotification);
    addAndMakeVisible (combo_box_large_);

    combo_box_disabled_.addItemList ({"Item 1", "Item 2", "Item 3"}, 1);
    combo_box_disabled_.setSelectedId (1, juce::dontSendNotification);
    combo_box_disabled_.setEnabled (false);
    addAndMakeVisible (combo_box_disabled_);
}

void Combos::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (combos_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (combo_box_small_).withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (combo_box_large_).withHeight (80.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (combo_box_disabled_).withHeight (40.f));

    layout.performLayout (getLocalBounds ());
}