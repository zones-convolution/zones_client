#include "IOPanel.h"

const std::string IOPanel::kDryWetKey = "dry_wet_parameter";
const std::string IOPanel::kInputGainKey = "input_gain_parameter";
const std::string IOPanel::kOutputGainKey = "output_gain_parameter";

IOPanel::IOPanel ()
{
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);
    addAndMakeVisible (input_gain_slider_);
    addAndMakeVisible (input_gain_label_);
    addAndMakeVisible (output_gain_slider_);
    addAndMakeVisible (output_gain_label_);

    dry_wet_label_.setText (juce::translate (kDryWetKey), juce::dontSendNotification);
    dry_wet_label_.setJustificationType (juce::Justification::centred);
    dry_wet_mix_slider_.setRange ({0.f, 1.f}, 0);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    dry_wet_mix_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateDryWetMix (static_cast<float> (dry_wet_mix_slider_.getValue ())));
    };

    input_gain_label_.setText (juce::translate (kInputGainKey), juce::dontSendNotification);
    input_gain_label_.setJustificationType (juce::Justification::centred);
    input_gain_slider_.setRange ({0.f, 2.f}, 0);
    input_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    input_gain_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateInputGain (static_cast<float> (input_gain_slider_.getValue ())));
    };

    output_gain_label_.setText (juce::translate (kOutputGainKey), juce::dontSendNotification);
    output_gain_label_.setJustificationType (juce::Justification::centred);
    output_gain_slider_.setRange ({0.f, 2.f}, 0);
    output_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    output_gain_slider_.onValueChange = [&]
    {
        //        realtime_parameter_context_.dispatch (
        //            UpdateOutputGain (static_cast<float> (output_gain_slider_.getValue ())));
    };
}

void IOPanel::resized ()
{
    juce::FlexBox slider_layout;
    slider_layout.flexDirection = juce::FlexBox::Direction::row;
    slider_layout.items.add (juce::FlexItem (dry_wet_mix_slider_).withFlex (1.f));
    slider_layout.items.add (LookAndFeel::kFlexSpacer);
    slider_layout.items.add (juce::FlexItem (input_gain_slider_).withFlex (1.f));
    slider_layout.items.add (LookAndFeel::kFlexSpacer);
    slider_layout.items.add (juce::FlexItem (output_gain_slider_).withFlex (1.f));

    juce::FlexBox label_layout;
    label_layout.flexDirection = juce::FlexBox::Direction::row;
    label_layout.items.add (juce::FlexItem (dry_wet_label_).withFlex (1.f));
    label_layout.items.add (LookAndFeel::kFlexSpacer);
    label_layout.items.add (juce::FlexItem (input_gain_label_).withFlex (1.f));
    label_layout.items.add (LookAndFeel::kFlexSpacer);
    label_layout.items.add (juce::FlexItem (output_gain_label_).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (juce::FlexItem (slider_layout).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (label_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
