#include "ZoneViewComponent.h"

ZoneViewComponent::ZoneViewComponent ()
{
    addAndMakeVisible (label_);
}

void ZoneViewComponent::resized ()
{
    label_.setBounds (getLocalBounds ());
}

void ZoneViewComponent::Update (const ZoneView & zone_view)
{
}
