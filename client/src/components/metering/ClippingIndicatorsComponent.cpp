
#include "ClippingIndicatorsComponent.h"

static constexpr auto kSpacing = 2.f;
static constexpr auto kMargin = 6.f;

void ClippingIndicator::paint (juce::Graphics & g)
{
    auto indicator_colour = is_clipping_ ? juce::Colours::red : juce::Colours::black;
    g.setColour (indicator_colour);
    g.fillRect (getLocalBounds ().toFloat ());
}

// void ClippingIndicator::resized ()
//{
//     auto indicator_colour = is_clipping_ ? juce::Colours::red : juce::Colours::black;
//     button_.setColour (juce::Label::backgroundColourId, indicator_colour);
//     button_.setBounds (getLocalBounds ());
// }

void ClippingIndicator::setFill (bool is_clipping)
{
    DBG ("setClippingto:" << (is_clipping ? "true" : "false"));
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

void ClippingIndicatorsComponent::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));
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
