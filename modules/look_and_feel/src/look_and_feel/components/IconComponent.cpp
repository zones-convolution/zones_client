#include "IconComponent.h"

void IconComponent::paint (juce::Graphics & g)
{
    g.setColour (colour_.has_value () ? colour_.value ()
                                      : findColour (juce::Label::ColourIds::textColourId));
    LookAndFeel::DrawBoxIcon (g, icon_, getLocalBounds ());
}

void IconComponent::SetIcon (const juce::String & icon)
{
    icon_ = icon;
}

void IconComponent::SetColour (const juce::Colour & colour)
{
    colour_ = colour;
}