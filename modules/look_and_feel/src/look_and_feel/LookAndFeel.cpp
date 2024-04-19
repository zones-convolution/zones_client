#include "LookAndFeel.h"

extern "C" const char assets_boxicons_ttf [];
extern "C" const unsigned assets_boxicons_ttf_size;

const juce::FlexItem LookAndFeel::kFlexSpacer =
    juce::FlexItem ().withHeight (kGap).withWidth (kGap);
const juce::FlexItem LookAndFeel::kDoubleFlexSpacer =
    juce::FlexItem ().withHeight (kDoubleGap).withWidth (kDoubleGap);

static const auto kBoxIconsTypeface =
    juce::Typeface::createSystemTypefaceFor (assets_boxicons_ttf, assets_boxicons_ttf_size);
static const juce::Font kBoxIconsFont {kBoxIconsTypeface};

LookAndFeel::LookAndFeel ()
{
    setColour (ColourIds::kPanel, juce::Colour (29, 32, 37));
    setColour (ColourIds::kPrimary, juce::Colour (149, 213, 178));
    setColour (ColourIds::kSecondary, juce::Colour (23, 26, 29));
    setColour (ColourIds::kDivider, juce::Colours::white.withAlpha (kDividerAlpha));

    setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (23, 26, 29));

    setColour (juce::ComboBox::backgroundColourId, findColour (ColourIds::kPanel).brighter (0.1f));
    setColour (juce::PopupMenu::highlightedBackgroundColourId,
               findColour (ColourIds::kPanel).brighter (0.2f));
    setColour (juce::PopupMenu::backgroundColourId, juce::Colours::transparentWhite);

    setColour (juce::TextButton::buttonColourId, findColour (ColourIds::kPanel));
    setColour (juce::Slider::trackColourId, findColour (ColourIds::kPrimary));

    auto default_font = juce::Font ("DM Sans", 20.f, juce::Font::FontStyleFlags::bold);
    setDefaultSansSerifTypeface (default_font.getTypefacePtr ());
}

void LookAndFeel::drawRotarySlider (juce::Graphics & g,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    float sliderPos,
                                    const float rotaryStartAngle,
                                    const float rotaryEndAngle,
                                    juce::Slider & slider)
{
    auto half_width = width * 0.5f;
    auto half_height = height * 0.5f;
    const float radius = juce::jmin (half_width, half_height);
    const float centre_x = x + half_width;
    const float centre_y = y + half_height;

    const float rx = centre_x - radius;
    const float ry = centre_y - radius;
    const float rw = radius * 2.0f;

    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    static constexpr float inner_circle_proportional_size = 0.92f;
    auto secondary_colour = slider.findColour (juce::ResizableWindow::backgroundColourId);
    g.setColour (secondary_colour);
    juce::Path filled_arc;
    filled_arc.addPieSegment (
        rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, inner_circle_proportional_size);
    g.fillPath (filled_arc);

    auto primary_colour = slider.findColour (ColourIds::kPrimary);
    g.setColour (primary_colour);
    juce::Path filled_arc_1;
    filled_arc_1.addPieSegment (
        rx, ry, rw, rw, rotaryStartAngle, angle, inner_circle_proportional_size);
    g.fillPath (filled_arc_1);
}

void LookAndFeel::drawPopupMenuBackground (juce::Graphics & g, int width, int height)
{
    auto popup_menu_panel_background_colour = findColour (juce::ComboBox::backgroundColourId);
    juce::Rectangle<float> bounds (0.f, 0.f, float (width), float (height));
    g.setColour (popup_menu_panel_background_colour);
    g.fillRoundedRectangle (bounds, kPopupMenuCornerRounding);
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
    juce::Rectangle<int> box_bounds (0, 0, width, height);

    auto background_colour = box.findColour (juce::ComboBox::backgroundColourId);
    if (box.isMouseOver () && box.isEnabled ())
        background_colour =
            background_colour.brighter (kHoverBrightnessMultiplier).withAlpha (kHoverAlpha);

    g.setColour (background_colour);
    g.fillRoundedRectangle (box_bounds.toFloat (), kComboBoxCornerRounding);

    auto font_height = juce::LookAndFeel_V4::getComboBoxFont (box).getHeight ();

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (juce::FlexItem ().withWidth (font_height).withHeight (font_height / 2.f));
    layout.items.add (kDoubleFlexSpacer);

    layout.performLayout (box_bounds);
    auto arrow_bounds = layout.items [0].currentBounds;

    juce::Path path;
    auto top_left = arrow_bounds.getTopLeft ();
    path.startNewSubPath (top_left);

    auto middle_bottom = juce::Point<float> (arrow_bounds.getCentreX (), arrow_bounds.getBottom ());
    path.lineTo (middle_bottom);

    auto top_right = arrow_bounds.getTopRight ();
    path.lineTo (top_right);

    const auto arrow_colour = box.findColour (juce::ComboBox::arrowColourId);

    g.setColour (arrow_colour.withAlpha ((box.isEnabled () ? 1.0f : 0.2f)));
    g.strokePath (path, juce::PathStrokeType (4.0f));
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
    if (button.getClickingTogglesState ())
        isButtonDown = button.getToggleState ();

    auto corner_size = kButtonCornerRounding;
    auto bounds = button.getLocalBounds ().toFloat ().reduced (0.5f, 0.5f);

    juce::Colour button_colour = button.findColour (juce::TextButton::ColourIds::buttonColourId);

    juce::Path button_path;
    button_path.addRoundedRectangle (bounds, corner_size);

    auto brightness = isButtonDown ? 0.32f : isMouseOverButton ? 0.16f : 0.08f;
    g.setColour (button_colour.brighter (brightness));
    g.fillPath (button_path);
}

juce::FlexItem LookAndFeel::ButtonFlexItem (juce::Component & button)
{
    return juce::FlexItem (button).withHeight (kButtonHeight);
}

juce::FlexItem LookAndFeel::LabelFlexItem (juce::Label & label)
{
    auto label_bounds = GetLabelBounds (label);

    return juce::FlexItem (label)
        .withHeight (label_bounds.getHeight ())
        .withWidth (label_bounds.getWidth ());
}

juce::FlexItem LookAndFeel::ComboFlexItem (juce::Component & combo)
{
    return juce::FlexItem (combo).withHeight (kComboHeight);
}

void LookAndFeel::drawBubble (juce::Graphics & graphics,
                              juce::BubbleComponent & component,
                              const juce::Point<float> & tip,
                              const juce::Rectangle<float> & body)
{
    auto & look_and_feel = component.getLookAndFeel ();
    auto bubble_colour = look_and_feel.findColour (juce::ResizableWindow::backgroundColourId);
    graphics.setColour (bubble_colour);

    graphics.fillRoundedRectangle (body.expanded (kPadding), kRounding);
}

juce::Rectangle<float> LookAndFeel::GetLabelBounds (juce::Label & label)
{
    auto & label_feel = label.getLookAndFeel ();
    auto label_font = label_feel.getLabelFont (label);
    auto border_size = label.getBorderSize ();

    auto label_width =
        label_font.getStringWidthFloat (label.getText ()) + border_size.getLeftAndRight ();
    auto label_height = label_font.getHeight () + border_size.getTopAndBottom ();

    return {label_width, label_height};
}

juce::FlexItem LookAndFeel::RotarySliderFlexItem (juce::Component & slider, float size)
{
    return juce::FlexItem (slider).withWidth (size).withHeight (size);
}

juce::FlexItem LookAndFeel::SmallIconFlexItem (juce::Component & icon)
{
    return juce::FlexItem (icon).withWidth (kSmallIconSize).withHeight (kSmallIconSize);
}

juce::FlexItem LookAndFeel::MediumIconFlexItem (juce::Component & icon)
{
    return juce::FlexItem (icon).withWidth (kMediumIconSize).withHeight (kMediumIconSize);
}

juce::FlexItem LookAndFeel::LargeIconFlexItem (juce::Component & icon)
{
    return juce::FlexItem (icon).withWidth (kLargeIconSize).withHeight (kLargeIconSize);
}

juce::FlexBox LookAndFeel::SliderLabelLayout (juce::Component & slider, juce::Label & label)
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::center;
    layout.alignItems = juce::FlexBox::AlignItems::center;

    layout.items.add (
        LookAndFeel::RotarySliderFlexItem (slider, LookAndFeel::kMediumRotarySliderSize));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::LabelFlexItem (label));

    return layout;
}

juce::FlexItem LookAndFeel::HorizontalDividerFlexItem (juce::Component & divider)
{
    return juce::FlexItem (divider).withHeight (kDividerThickness);
}

juce::FlexItem LookAndFeel::VerticalDividerFlexItem (juce::Component & divider)
{
    return juce::FlexItem (divider).withWidth (kDividerThickness);
}

void LookAndFeel::DrawBoxIcon (juce::Graphics & graphics,
                               const juce::String & icon,
                               const juce::Rectangle<int> & bounds)
{
    graphics.setFont (kBoxIconsFont);
    graphics.setFont (juce::jmin (bounds.getHeight (), bounds.getWidth ()));
    graphics.drawFittedText (icon, bounds, juce::Justification::centred, 1);
}
