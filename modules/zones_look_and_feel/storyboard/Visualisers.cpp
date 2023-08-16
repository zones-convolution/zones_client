#include "Visualisers.h"

Visualisers::Visualisers ()
{
    visualisers_label_.setText ("Visualisers", juce::dontSendNotification);
    visualisers_label_.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (graph3d_component_);
}

void Visualisers::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (visualisers_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (graph3d_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}