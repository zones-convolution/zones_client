#include "ClippingIndicator.h"

void ClippingIndicator::paint (juce::Graphics & g)
{
    auto background_colour =
        getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId);
    g.fillAll (is_clipping_ ? juce::Colours::red : background_colour);
}

void ClippingIndicator::setFill (bool is_clipping)
{
    is_clipping_ = is_clipping;
}
