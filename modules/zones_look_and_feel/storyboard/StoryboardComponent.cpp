#include "StoryboardComponent.h"

StoryboardComponent::StoryboardComponent ()
{
    static constexpr int kDefaultWindowWidth = 1920;
    static constexpr int kDefaultWindowHeight = 1080;

    setSize (kDefaultWindowWidth, kDefaultWindowHeight);

    addAndMakeVisible (button_panel_);
    addAndMakeVisible (combos_panel_);
}

void StoryboardComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.alignContent = juce::FlexBox::AlignContent::center;

    layout.items.add (juce::FlexItem (button_panel_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (combos_panel_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

void StoryboardComponent::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
}