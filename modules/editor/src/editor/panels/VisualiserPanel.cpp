#include "VisualiserPanel.h"

VisualiserPanel::VisualiserPanel ()
{
    addAndMakeVisible (waterfall_panel_);
}

void VisualiserPanel::resized ()
{
    juce::FlexBox flex_box;
    flex_box.flexDirection = juce::FlexBox::Direction::column;
    flex_box.items.add (juce::FlexItem (waterfall_panel_).withFlex (1.f));
    flex_box.performLayout (getLocalBounds ().toFloat ());
}
