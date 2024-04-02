#include "IrEngineComponent.h"

const std::string IrEngineComponent::kIrEnginePanelKey = "ir_engine_panel";

IrEngineComponent::IrEngineComponent (juce::AudioProcessorValueTreeState & parameter_tree)
    : room_size_attachment_ (parameter_tree, ParameterTree::kRoomSizeParameterId, room_size_slider_)
    , reverb_time_attachment_ (parameter_tree,
                               ParameterTree::kReverbTimeParameterId,
                               reverb_time_slider_)
    , resampler_attachment_ (parameter_tree,
                             ParameterTree::kResamplerParameterId,
                             resampler_slider_)

{
    addAndMakeVisible (ir_engine_label_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (room_size_slider_);
    addAndMakeVisible (room_size_label_);
    addAndMakeVisible (reverb_time_slider_);
    addAndMakeVisible (reverb_time_label_);
    addAndMakeVisible (resampler_slider_);
    addAndMakeVisible (resampler_label_);

    ir_engine_label_.setText (juce::translate (kIrEnginePanelKey), juce::dontSendNotification);

    room_size_label_.setText (juce::translate (ParameterTree::kRoomSizeParameterId),
                              juce::dontSendNotification);
    room_size_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());

    reverb_time_label_.setText (juce::translate (ParameterTree::kReverbTimeParameterId),
                                juce::dontSendNotification);
    reverb_time_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());

    resampler_label_.setText (juce::translate (ParameterTree::kResamplerParameterId),
                              juce::dontSendNotification);
    resampler_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
}

void IrEngineComponent::resized ()
{
    auto room_size_layout = LookAndFeel::SliderLabelLayout (room_size_slider_, room_size_label_);
    auto reverb_time_layout =
        LookAndFeel::SliderLabelLayout (reverb_time_slider_, reverb_time_label_);
    auto resampler_layout = LookAndFeel::SliderLabelLayout (resampler_slider_, resampler_label_);

    juce::FlexBox parameter_layout;
    parameter_layout.flexDirection = juce::FlexBox::Direction::row;

    parameter_layout.items.add (juce::FlexItem (resampler_layout).withFlex (1.f));
    parameter_layout.items.add (juce::FlexItem (room_size_layout).withFlex (1.f));
    parameter_layout.items.add (juce::FlexItem (reverb_time_layout).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (ir_engine_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameter_layout).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
