#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LookAndFeel : public juce::LookAndFeel_V4
{
public:
    LookAndFeel ();

    static constexpr float kGap = 8.f;
    static constexpr float kHalfGap = 0.5f * kGap;
    static constexpr float kDoubleGap = 2.f * kGap;

    static constexpr float kPadding = 8.f;
    static constexpr float kRounding = 8.f;

    static constexpr float kComboBoxCornerRounding = kRounding;
    static constexpr float kPopupMenuCornerRounding = kRounding;
    static constexpr float kButtonCornerRounding = kRounding;

    static constexpr float kComponentInset = 4.f;

    static constexpr float kDividerAlpha = 0.1f;
    static constexpr float kDividerThickness = 2.f;

    static constexpr float kHoverAlpha = 0.6f;
    static constexpr float kHoverBrightnessMultiplier = 0.1f;

    static const juce::FlexItem kFlexSpacer;
    static const juce::FlexItem kDoubleFlexSpacer;

    static constexpr auto kButtonHeight = 40.f;
    static juce::FlexItem ButtonFlexItem (juce::Component & button);

    static juce::FlexItem LabelFlexItem (juce::Label & label);

    static juce::FlexItem HorizontalDividerFlexItem (juce::Component & divider);
    static juce::FlexItem VerticalDividerFlexItem (juce::Component & divider);

    static constexpr auto kComboHeight = 40.f;
    static juce::FlexItem ComboFlexItem (juce::Component & combo);

    static constexpr auto kMediumRotarySliderSize = 80.f;
    static juce::FlexItem RotarySliderFlexItem (juce::Component & slider, float size);

    static juce::FlexBox SliderLabelLayout (juce::Component & slider, juce::Label & label);

    static juce::Rectangle<float> GetLabelBounds (juce::Label & label);

    enum ColourIds
    {
        kPanel,
        kPrimary,
        kSecondary,
        kDivider
    };
    void drawBubble (juce::Graphics & graphics,
                     juce::BubbleComponent & component,
                     const juce::Point<float> & tip,
                     const juce::Rectangle<float> & body) override;
    juce::Font getTextButtonFont (juce::TextButton & button, int buttonHeight) override;

    juce::Font getLabelFont (juce::Label & label) override;

    void drawRotarySlider (juce::Graphics & g,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPos,
                           const float rotaryStartAngle,
                           const float rotaryEndAngle,
                           juce::Slider &) override;

    void drawButtonBackground (juce::Graphics & graphics,
                               juce::Button & button,
                               const juce::Colour & backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawPopupMenuBackground (juce::Graphics & g, int width, int height) override;

    void getIdealPopupMenuItemSize (const juce::String & text,
                                    const bool isSeparator,
                                    int standardMenuItemHeight,
                                    int & idealWidth,
                                    int & idealHeight) override;

    void drawComboBox (juce::Graphics &,
                       int width,
                       int height,
                       bool isButtonDown,
                       int buttonX,
                       int buttonY,
                       int buttonW,
                       int buttonH,
                       juce::ComboBox &) override;

    void positionComboBoxText (juce::ComboBox & box, juce::Label & label) override;

    juce::Label * createComboBoxTextBox (juce::ComboBox & combo) override;

    juce::PopupMenu::Options getOptionsForComboBoxPopupMenu (juce::ComboBox & box,
                                                             juce::Label & label) override;
};