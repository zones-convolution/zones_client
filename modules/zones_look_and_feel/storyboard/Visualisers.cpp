#include "Visualisers.h"

Visualisers::Visualisers ()
{
    visualisers_label_.setText ("Visualisers", juce::dontSendNotification);
    visualisers_label_.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (visualisers_label_);
    addAndMakeVisible (spectrogram_visualiser_component_);
    spectrogram_visualiser_component_.Start ();

    addAndMakeVisible (graph_component_);
}

void Visualisers::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (visualisers_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (spectrogram_visualiser_component_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (graph_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ());
}