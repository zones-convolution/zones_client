#include "SidebarHeader.h"

SidebarHeader::SidebarHeader ()
{
    addAndMakeVisible (logo_component_);
}

void SidebarHeader::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (logo_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
