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
    auto ir_metadata = zone_view.ir_metadata;

    if (ir_metadata.name)
        label_.setText (*ir_metadata.name, juce::dontSendNotification);
}
