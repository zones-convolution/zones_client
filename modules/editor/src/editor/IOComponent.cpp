#include "IOComponent.h"

#include "look_and_feel/LookAndFeel.h"

const std::string IOComponent::kIOPanelKey = "io_panel";

IOComponent::IOComponent (juce::AudioProcessorValueTreeState & parameter_tree,
                          PlayerController & player_controller)
    : dry_wet_mix_attachment_ (parameter_tree,
                               ParameterTree::kDryWetMixParameterId,
                               dry_wet_mix_slider_)
    , input_gain_attachment_ (parameter_tree,
                              ParameterTree::kInputGainParameterId,
                              input_gain_slider_)
    , output_gain_attachment_ (parameter_tree,
                               ParameterTree::kOutputGainParameterId,
                               output_gain_slider_)
    , player_component_ (player_controller)
{
    io_label_.setText (juce::translate (kIOPanelKey), juce::dontSendNotification);
    addAndMakeVisible (io_label_);
    addAndMakeVisible (top_divider_);

    addAndMakeVisible (player_panel_);

    dry_wet_label_.setText (juce::translate (ParameterTree::kDryWetMixParameterId),
                            juce::dontSendNotification);
    dry_wet_label_.setJustificationType (juce::Justification::centred);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);

    input_gain_label_.setText (juce::translate (ParameterTree::kInputGainParameterId),
                               juce::dontSendNotification);
    input_gain_label_.setJustificationType (juce::Justification::centred);
    input_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    addAndMakeVisible (input_gain_slider_);
    addAndMakeVisible (input_gain_label_);

    output_gain_label_.setText (juce::translate (ParameterTree::kOutputGainParameterId),
                                juce::dontSendNotification);
    output_gain_label_.setJustificationType (juce::Justification::centred);
    output_gain_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    addAndMakeVisible (output_gain_slider_);
    addAndMakeVisible (output_gain_label_);
}

void IOComponent::resized ()
{
    auto dry_wet_layout = LookAndFeel::SliderLabelLayout (dry_wet_mix_slider_, dry_wet_label_);
    auto input_gain_layout = LookAndFeel::SliderLabelLayout (input_gain_slider_, input_gain_label_);
    auto output_gain_layout =
        LookAndFeel::SliderLabelLayout (output_gain_slider_, output_gain_label_);

    juce::FlexBox top_layout;
    top_layout.flexDirection = juce::FlexBox::Direction::row;
    top_layout.items.add (juce::FlexItem (player_panel_).withFlex (1.f));
    top_layout.items.add (juce::FlexItem ({}).withFlex (1.f));

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
    layout.items.add (juce::FlexItem (top_layout).withFlex (1.5f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameter_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
