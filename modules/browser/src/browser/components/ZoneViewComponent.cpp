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
    auto & zone_metadata = zone_view.zone_metadata;
    label_.setText (zone_metadata.title, juce::dontSendNotification);
}
