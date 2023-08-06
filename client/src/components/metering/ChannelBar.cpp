#include "ChannelBar.h"

const std::pair<juce::Colour, juce::Colour> MeterBar::kMeterGradientColours = {
    juce::Colour::fromRGB (255, 149, 0),
    juce::Colour::fromRGB (75, 217, 102)};

void MeterBar::paint (juce::Graphics & g)
{
    g.setGradientFill (juce::ColourGradient::vertical (
        kMeterGradientColours.first, kMeterGradientColours.second, getLocalBounds ()));

    static constexpr auto kPeakBarHeight = 2.f;
    auto peak_bar =
        juce::Rectangle<float> (0.f,
                                (1.f - peak_target_value_) * static_cast<float> (getHeight ()),
                                static_cast<float> (getWidth ()),
                                kPeakBarHeight);
    g.fillRect (peak_bar);

    auto main_bar_bounds = getLocalBounds ().toFloat ().withTrimmedTop (
        (1.f - fill_) * static_cast<float> (getHeight ()));
    g.fillRect (main_bar_bounds);
}

void MeterBar::SetFill (float fill)
{
    jassert (fill >= 0.0f && fill <= 1.0f);
    fill_ = fill;
}

void MeterBar::SetPeak (float peak)
{
    jassert (peak >= 0.0f && peak <= 1.0f);
    peak_target_value_ = peak;
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
    for (auto level_index = 1; level_index < kNumDiscreteLevels - 1; ++level_index)
        g.fillRect (layout.items [level_index].currentBounds.toFloat ());
}

ChannelBar::ChannelBar ()
{
    addAndMakeVisible (meter_bar_);
    addAndMakeVisible (discrete_level_bars_);
    discrete_level_bars_.setAlpha (0.1f);
}

void ChannelBar::resized ()
{
    meter_bar_.setBounds (getLocalBounds ());
    discrete_level_bars_.setBounds (getLocalBounds ());
}

void ChannelBar::paint (juce::Graphics & g)
{
    g.fillAll (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));
}

void ChannelBar::SetTarget (float target, float peak)
{
    meter_bar_.SetFill (juce::jlimit (0.f, 1.f, target));
    meter_bar_.SetPeak (juce::jlimit (0.f, 1.f, peak));
}
