#include "ChannelBar.h"

const std::pair<juce::Colour, juce::Colour> MeterBar::kMeterGradientColours = {
    juce::Colour::fromRGB (255, 149, 0),
    juce::Colour::fromRGB (75, 217, 102)};

void MeterBar::paint (juce::Graphics & g)
{
    g.setGradientFill (juce::ColourGradient::vertical (
        kMeterGradientColours.first, kMeterGradientColours.second, getLocalBounds ()));
    g.fillAll ();
}

void DiscreteLevelBars::paint (juce::Graphics & g)
{
    static constexpr auto kNumDiscreteLevels = 8;
    static constexpr auto kBarHeight = 2.f;

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    for (auto level_index = 0; level_index < kNumDiscreteLevels; ++level_index)
        layout.items.add (juce::FlexItem ().withHeight (kBarHeight));

    layout.performLayout (getLocalBounds ().toFloat ());

    g.setColour (juce::Colours::white);

    for (auto & layout_line : layout.items)
        g.fillRect (layout_line.currentBounds.toFloat ());
}

ChannelBar::ChannelBar ()
{
    addAndMakeVisible (meter_bar_);
    addAndMakeVisible (discrete_level_bars_);
}
void ChannelBar::resized ()
{
    meter_bar_.setBounds (getLocalBounds ());
    discrete_level_bars_.setBounds (getLocalBounds ());
}
