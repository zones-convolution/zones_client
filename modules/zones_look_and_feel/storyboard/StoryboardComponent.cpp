#include "StoryboardComponent.h"

StoryboardComponent::StoryboardComponent ()
{
    static constexpr int kDefaultWindowWidth = 1920;
    static constexpr int kDefaultWindowHeight = 1080;

    setSize (kDefaultWindowWidth, kDefaultWindowHeight);
}

void StoryboardComponent::resized ()
{
}

void StoryboardComponent::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
}