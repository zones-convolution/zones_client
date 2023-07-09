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

void LookAndFeel::drawRotarySlider (juce::Graphics & g,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    float sliderPos,
                                    const float rotaryStartAngle,
                                    const float rotaryEndAngle,
                                    juce::Slider &)
{
    auto halfWidth = width * 0.5f;
    auto halfHeight = height * 0.5f;
    const float radius = juce::jmin (halfWidth, halfHeight);
    const float centreX = x + halfWidth;
    const float centreY = y + halfHeight;

    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;

    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    static constexpr float innerCircleProportionalSize = 0.92f;

    auto secondaryGrad = juce::ColourGradient::horizontal (
        Pallette::secondary.darker (0.1f), 0.0f, Pallette::secondary.brighter (0.1f), width);
    g.setGradientFill (secondaryGrad);
    juce::Path filledArc;
    filledArc.addPieSegment (
        rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, innerCircleProportionalSize);
    g.fillPath (filledArc);

    auto primaryGrad = juce::ColourGradient::horizontal (
        Pallette::primary.darker (0.1f), 0.0f, Pallette::primary.brighter (0.1f), width);
    g.setGradientFill (primaryGrad);
    juce::Path filledArc1;
    filledArc1.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, innerCircleProportionalSize);
    g.fillPath (filledArc1);
}

void LookAndFeel::drawPopupMenuBackground (juce::Graphics & g, int width, int height)
{
    auto popupMenuPanelBackgroundColour = findColour (ColourID::kPopupMenuPanelBackgroundId);
    juce::Rectangle<float> bounds (0.f, 0.f, float (width), float (height));
    g.setColour (popupMenuPanelBackgroundColour);
    g.fillRoundedRectangle (bounds, kCallOutBoxCornerRounding);
}

void LookAndFeel::getIdealPopupMenuItemSize (const juce::String & text,
                                             const bool isSeparator,
                                             int standardMenuItemHeight,
                                             int & idealWidth,
                                             int & idealHeight)
{
    if (isSeparator)
        return LookAndFeel_V4::getIdealPopupMenuItemSize (
            text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight);

    auto font = getPopupMenuFont ();
    idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight
                                             : juce::roundToInt (font.getHeight () * 1.5f);
    idealWidth = font.getStringWidth (text) + idealHeight * 2;
}

void LookAndFeel::drawComboBox (juce::Graphics & g,
                                int width,
                                int height,
                                bool,
                                int,
                                int,
                                int,
                                int,
                                juce::ComboBox & box)
{
    juce::Rectangle<int> boxBounds (0, 0, width, height);

    auto backgroundColour = box.findColour (juce::ComboBox::backgroundColourId);
    if (box.isMouseOver () && box.isEnabled ())
        backgroundColour =
            backgroundColour.brighter (kHoverBrightnessMultiplier).withAlpha (kHoverAlpha);

    g.setColour (backgroundColour);
    g.fillRoundedRectangle (boxBounds.toFloat (), kComboBoxCornerRounding);

    juce::Rectangle<int> arrowZone (width - height - 4, 0, height, height);
    arrowZone.reduce (kComponentInset, kComponentInset);
    auto floatArrowZone = arrowZone.toFloat ();
    juce::Path path;
    path.startNewSubPath (floatArrowZone.getX (),
                          floatArrowZone.getCentreY () - (floatArrowZone.getHeight () / 4));
    path.lineTo (floatArrowZone.getCentreX (),
                 floatArrowZone.getCentreY () + (floatArrowZone.getHeight () / 4));
    path.lineTo (floatArrowZone.getRight (),
                 floatArrowZone.getCentreY () - (floatArrowZone.getHeight () / 4));

    const auto arrowColour = box.findColour (juce::ComboBox::arrowColourId);

    g.setColour (arrowColour.withAlpha ((box.isEnabled () ? 1.0f : 0.2f)));
    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void LookAndFeel::positionComboBoxText (juce::ComboBox & box, juce::Label & label)
{
    label.setBounds (kComponentInset,
                     0,
                     box.getWidth () - box.getHeight () - 4 - kComponentInset,
                     box.getHeight ());
    label.setFont (getComboBoxFont (box));
}

juce::Label * LookAndFeel::createComboBoxTextBox (juce::ComboBox & combo)
{
    juce::Label * label = new juce::Label ("", "");
    label->setColour (juce::Label::ColourIds::textColourId,
                      (combo.findColour (juce::ComboBox::ColourIds::textColourId)));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setMinimumHorizontalScale (1.0f);
    label->setInterceptsMouseClicks (false, false);
    return label;
}

juce::PopupMenu::Options LookAndFeel::getOptionsForComboBoxPopupMenu (juce::ComboBox & box,
                                                                      juce::Label & label)
{
    juce::ignoreUnused (label);
    return juce::PopupMenu::Options ()
        .withTargetComponent (&box)
        .withItemThatMustBeVisible (box.getSelectedId ())
        .withMinimumWidth (box.getWidth ())
        .withMaximumNumColumns (1);
}

void LookAndFeel::drawButtonBackground (juce::Graphics & g,
                                        juce::Button & button,
                                        const juce::Colour &,
                                        bool isMouseOverButton,
                                        bool isButtonDown)
{
    static constexpr float kButtonStrokeThickness = 1.f;
    if (button.getClickingTogglesState ())
        isButtonDown = button.getToggleState ();

    auto cornerSize = kButtonCornerRounding;
    auto bounds = button.getLocalBounds ().toFloat ().reduced (0.5f, 0.5f);

    auto highlight = button.findColour (juce::TextButton::ColourIds::buttonOnColourId);

    juce::Colour button_colour = button.findColour (juce::TextButton::ColourIds::buttonColourId);
    //    if (isButtonDown)
    //        background_colour = highlight;
    //    if (button.getToggleState ())
    //        background_colour = highlight;
    //    if (isMouseOverButton)
    //        background_colour = background_colour.brighter (0.25f);

    juce::Path button_path;
    button_path.addRoundedRectangle (bounds, cornerSize);

    g.setColour (button_colour);
    g.strokePath (button_path, juce::PathStrokeType (kButtonStrokeThickness));

    auto fill_alpha = isButtonDown ? 0.2f : isMouseOverButton ? 0.1f : 0.f;
    g.setColour (button_colour.withAlpha (fill_alpha));
    g.fillPath (button_path);
}