#include "DividerComponent.h"

void DividerComponent::paint (juce::Graphics & g)
{
    g.fillAll (findColour (LookAndFeel::ColourIds::kDivider));
}
