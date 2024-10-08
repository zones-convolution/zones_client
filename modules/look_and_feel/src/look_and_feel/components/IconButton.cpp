#include "IconButton.h"

#include "look_and_feel/LookAndFeel.h"

IconButton::IconButton ()
    : IconButton (juce::String ())
{
}

IconButton::IconButton (const juce::String & name)
    : Button (name)
{
}

IconButton::IconButton (const juce::String & name, const juce::String & icon)
    : IconButton (name)
{
    SetIcon (icon);
}

IconButton::IconButton (const juce::String & name,
                        const juce::String & icon,
                        const juce::String & tool_tip)
    : IconButton (name, icon)
{
    setTooltip (tool_tip);
}

void IconButton::paintButton (juce::Graphics & g,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown)
{
    auto & look_and_feel = getLookAndFeel ();
    auto is_toggled = getToggleState ();

    look_and_feel.drawButtonBackground (g,
                                        *this,
                                        findColour (is_toggled ? juce::TextButton::buttonOnColourId
                                                               : juce::TextButton::buttonColourId),
                                        shouldDrawButtonAsHighlighted,
                                        shouldDrawButtonAsDown);

    g.setColour (look_and_feel.findColour (juce::Label::ColourIds::textColourId)
                     .withMultipliedAlpha (isEnabled () ? 1.0f : 0.5f));
    LookAndFeel::DrawBoxIcon (g, icon_, getLocalBounds ().reduced (LookAndFeel::kComponentInset));

    if (is_toggled)
    {
        g.setColour (findColour (LookAndFeel::ColourIds::kPrimary));
        g.drawRoundedRectangle (
            getLocalBounds ().toFloat (), LookAndFeel::kComboBoxCornerRounding, 2.f);
    }
}

void IconButton::SetIcon (const juce::String & icon)
{
    icon_ = icon;
    repaint ();
}
