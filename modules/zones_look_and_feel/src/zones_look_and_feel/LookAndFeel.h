#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LookAndFeel : public juce::LookAndFeel_V4
{
public:
    LookAndFeel ();

    static constexpr float kGap = 4.f;
    static constexpr float kHalfGap = 0.5f * kGap;
    static constexpr float kDoubleGap = 2.f * kGap;

    static constexpr float kPadding = 8.f;
    static constexpr float kRounding = 8.f;

    static const juce::FlexItem kFlexSpacer;
    static const juce::FlexItem kDoubleFlexSpacer;

    enum ColourIds
    {
        kPanel,
        kPrimary,
        kSecondary
    };

    juce::Font getLabelFont (juce::Label & label) override;
};