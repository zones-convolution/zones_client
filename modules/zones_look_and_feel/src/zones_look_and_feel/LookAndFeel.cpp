#include "LookAndFeel.h"

const juce::FlexItem LookAndFeel::kFlexSpacer =
    juce::FlexItem ().withHeight (kGap).withWidth (kGap);
const juce::FlexItem LookAndFeel::kDoubleFlexSpacer =
    juce::FlexItem ().withHeight (kDoubleGap).withWidth (kDoubleGap);

LookAndFeel::LookAndFeel ()
{
    setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (23, 26, 29));
    setColour (ColourIds::kPanel, juce::Colour (29, 32, 37));
    setColour (ColourIds::kPrimary, juce::Colour (149, 213, 178));
    setColour (ColourIds::kSecondary, juce::Colours::pink);

    setColour (juce::Slider::trackColourId, findColour (ColourIds::kPrimary));
    setDefaultSansSerifTypefaceName ("DM Sans");
}

juce::Font LookAndFeel::getLabelFont (juce::Label & label)
{
    return juce::Font (20.f, juce::Font::FontStyleFlags::bold);
}
