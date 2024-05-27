#include "EQComponent.h"

const std::string EQComponent::kEQPanelKey = "eq_panel";

EQComponent::EQComponent (juce::AudioProcessorValueTreeState & parameter_tree)
    : bass_attachment_ (parameter_tree, ParameterTree::kBassParameterId, bass_slider_)
    , treble_attachment_ (parameter_tree, ParameterTree::kTrebleParameterId, treble_slider_)
{
    eq_label_.setText (juce::translate (kEQPanelKey), juce::dontSendNotification);
    addAndMakeVisible (eq_label_);
    addAndMakeVisible (divider_);

    bass_label_.setText (juce::translate (ParameterTree::kBassParameterId),
                         juce::dontSendNotification);
    bass_label_.setJustificationType (juce::Justification::centred);
    bass_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    addAndMakeVisible (bass_slider_);
    addAndMakeVisible (bass_label_);

    treble_label_.setText (juce::translate (ParameterTree::kTrebleParameterId),
                           juce::dontSendNotification);
    treble_label_.setJustificationType (juce::Justification::centred);
    treble_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    addAndMakeVisible (treble_slider_);
    addAndMakeVisible (treble_label_);
}
void EQComponent::resized ()
{
    auto bass_layout = LookAndFeel::SliderLabelLayout (bass_slider_, bass_label_);
    auto treble_layout = LookAndFeel::SliderLabelLayout (treble_slider_, treble_label_);

    juce::FlexBox parameter_layout;
    parameter_layout.flexDirection = juce::FlexBox::Direction::row;

    parameter_layout.items.add (juce::FlexItem (bass_layout).withFlex (1.f));
    parameter_layout.items.add (juce::FlexItem (treble_layout).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (eq_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameter_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
