
#include "ClippingIndicatorsComponent.h"

static constexpr auto kSpacing = 2.f;

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

ClippingIndicatorsComponent::ClippingIndicatorsComponent ()
{
    for (auto & clipping_indicator : clipping_indicators_)
    {
        addAndMakeVisible (clipping_indicator);
        clipping_indicator.onClick = [&] { clipping_indicator.setFill (false); };
    }
}

void ClippingIndicatorsComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    for (auto & clipping_indicator : clipping_indicators_)
    {
        layout.items.add (juce::FlexItem (clipping_indicator).withFlex (1.f));
        layout.items.add (juce::FlexItem ().withWidth (kSpacing));
    }
    layout.items.removeLast ();

    layout.performLayout (getLocalBounds ().toFloat ());
}
void ClippingIndicatorsComponent::SetIndicator (size_t channel_index, bool is_clipping)
{
    clipping_indicators_ [channel_index].setFill (is_clipping);
}