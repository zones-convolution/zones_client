#include "SidebarHeader.h"

SidebarHeader::SidebarHeader ()
{
    addAndMakeVisible (logo_component_);
}

void SidebarHeader::resized ()
{
    logo_component_.setBounds (
        getLocalBounds ().toFloat ().reduced (0.f, LookAndFeel::kHalfGap).toNearestInt ());
}
