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

DiscreteLevelBars::DiscreteLevelBars (LabelContainer & label_container)
    : label_container_ (label_container)
{
}

void DiscreteLevelBars::paint (juce::Graphics & g)
{
    auto width = getWidth ();
    static constexpr auto kBarHeight = 2;
    auto height = getHeight ();

    g.setColour (juce::Colours::white);
    auto label_heights = label_container_.getHeightsWithinBounds (getLocalBounds ());

    for (auto & label_height : label_heights)
    {
        g.fillRect (0, label_height, width, kBarHeight);
    }
}

ChannelBar::ChannelBar (LabelContainer & label_height_container)
    : discrete_level_bars_ (label_height_container)
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
