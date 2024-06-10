//
// Created by Micah Strange on 30/05/2024.
//

#include "RTComponent.h"

RTComponent::RTComponent (juce::AudioProcessorValueTreeState & parameter_tree,
                          PlayerController & player_controller)
    : player_component_ (player_controller)
    , eq_component_ (parameter_tree)
    , io_component_ (parameter_tree)
{
    addAndMakeVisible (player_panel_);
    addAndMakeVisible (eq_panel_);
    addAndMakeVisible (io_panel_);
}

void RTComponent::resized ()
{
    juce::FlexBox top_layout;
    top_layout.flexDirection = juce::FlexBox::Direction::row;
    top_layout.items.add (juce::FlexItem (player_panel_).withFlex (1.f));
    top_layout.items.add (juce::FlexItem (eq_panel_).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (juce::FlexItem (top_layout).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (io_panel_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}