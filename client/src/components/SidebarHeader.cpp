#include "SidebarHeader.h"

SidebarHeader::SidebarHeader ()
{
    addAndMakeVisible (logo_component_);
    addAndMakeVisible (icon_sad_);

    icon_sad_.SetIcon (BoxIcons::kBxHappy);
}

void SidebarHeader::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (logo_component_).withFlex (1.f));
    layout.items.add (juce::FlexItem (icon_sad_).withWidth (20.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
