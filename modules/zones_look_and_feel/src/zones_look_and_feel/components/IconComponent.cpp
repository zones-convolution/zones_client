#include "IconComponent.h"

void IconComponent::paint (juce::Graphics & g)
{
    auto label_colour = findColour (juce::Label::ColourIds::textColourId);
    g.setColour (label_colour);
    LookAndFeel::DrawBoxIcon (g, icon_, getLocalBounds ());
}

void IconComponent::SetIcon (const juce::String & icon)
{
    icon_ = icon;
}
