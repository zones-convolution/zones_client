#include "ChannelBar.h"

const std::pair<juce::Colour, juce::Colour> MeterBar::kMeterGradientColours = {
    juce::Colour::fromRGB (255, 149, 0),
    juce::Colour::fromRGB (75, 217, 102)};

void MeterBar::paint (juce::Graphics & g)
{
    g.setGradientFill (juce::ColourGradient::vertical (
        kMeterGradientColours.first, kMeterGradientColours.second, getLocalBounds ()));

    auto peak_bar = juce::Rectangle<float> (
        0.f, (1 - highest_value_) * static_cast<float> (getHeight ()), getWidth (), 2.f);
    g.fillRect (peak_bar);

    auto fill_bounds = getLocalBounds ().toFloat ().withTrimmedTop (
        (1.f - fill_) * static_cast<float> (getHeight ()));
    g.fillRect (fill_bounds);
}

void MeterBar::SetFill (float fill)
{
    jassert (fill >= 0.0f && fill <= 1.0f);
    fill_ = fill;

    if (fill >= highest_value_)
    {
        highest_value_ = fill;
        if (isTimerRunning ())
            stopTimer ();
    }
    else
    {
        if (! isTimerRunning ())
            startTimer (2000);
    }
}
void MeterBar::timerCallback ()
{
    highest_value_ = fill_;
    stopTimer ();
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

    smoothed_level_.reset (44100, 5);
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

void ChannelBar::SetTarget (float target)
{
    smoothed_level_.skip (44100 * 0.03);
    if (target < smoothed_level_.getCurrentValue ())
    {
        smoothed_level_.setTargetValue (target);
        meter_bar_.SetFill (smoothed_level_.getNextValue ());
    }
    else
    {
        smoothed_level_.setCurrentAndTargetValue (target);
        meter_bar_.SetFill (target);
    }
}
