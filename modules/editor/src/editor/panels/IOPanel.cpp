#include "IOPanel.h"

const std::string IOPanel::kIOPanelKey = "io_panel";
const std::string IOPanel::kDryWetKey = "dry_wet_parameter";
const std::string IOPanel::kInputGainKey = "input_gain_parameter";
const std::string IOPanel::kOutputGainKey = "output_gain_parameter";

IOPanel::IOPanel (lager::context<RealtimeParameterAction> & realtime_parameter_context)
{
    addAndMakeVisible (io_label_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);
    addAndMakeVisible (input_gain_slider_);
    addAndMakeVisible (input_gain_label_);
    addAndMakeVisible (output_gain_slider_);
    addAndMakeVisible (output_gain_label_);

    io_label_.setText (juce::translate (kIOPanelKey), juce::dontSendNotification);

    dry_wet_label_.setText (juce::translate (kDryWetKey), juce::dontSendNotification);
    dry_wet_label_.setJustificationType (juce::Justification::centred);
    dry_wet_mix_slider_.setRange ({0.f, 1.f}, 0);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    dry_wet_mix_slider_.onValueChange = [&]
    {
        realtime_parameter_context.dispatch (
            UpdateDryWetMix (static_cast<float> (dry_wet_mix_slider_.getValue ())));
    };

    input_gain_label_.setText (juce::translate (kInputGainKey), juce::dontSendNotification);
    input_gain_label_.setJustificationType (juce::Justification::centred);
    input_gain_slider_.setRange ({0.f, 2.f}, 0);
    input_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    input_gain_slider_.onValueChange = [&]
    {
        realtime_parameter_context.dispatch (
            UpdateInputGain (static_cast<float> (input_gain_slider_.getValue ())));
    };

    output_gain_label_.setText (juce::translate (kOutputGainKey), juce::dontSendNotification);
    output_gain_label_.setJustificationType (juce::Justification::centred);
    output_gain_slider_.setRange ({0.f, 2.f}, 0);
    output_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    output_gain_slider_.onValueChange = [&]
    {
        realtime_parameter_context.dispatch (
            UpdateOutputGain (static_cast<float> (output_gain_slider_.getValue ())));
    };
}

void IOPanel::resized ()
{
    auto dry_wet_layout = LookAndFeel::SliderLabelLayout (dry_wet_mix_slider_, dry_wet_label_);
    auto input_gain_layout = LookAndFeel::SliderLabelLayout (input_gain_slider_, input_gain_label_);
    auto output_gain_layout =
        LookAndFeel::SliderLabelLayout (output_gain_slider_, output_gain_label_);

    juce::FlexBox parameter_layout;
    parameter_layout.flexDirection = juce::FlexBox::Direction::row;

    parameter_layout.items.add (juce::FlexItem (dry_wet_layout).withFlex (1.f));
    parameter_layout.items.add (juce::FlexItem (input_gain_layout).withFlex (1.f));
    parameter_layout.items.add (juce::FlexItem (output_gain_layout).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (io_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameter_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
