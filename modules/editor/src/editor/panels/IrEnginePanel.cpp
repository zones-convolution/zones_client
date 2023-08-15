#include "IrEnginePanel.h"

IrEnginePanel::IrEnginePanel (lager::context<IrEngineParameterAction> & ir_engine_parameter_context)
{
    addAndMakeVisible (ir_engine_label_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (room_size_slider_);
    addAndMakeVisible (room_size_label_);
    addAndMakeVisible (reverb_time_slider_);
    addAndMakeVisible (reverb_time_label_);

    ir_engine_label_.setText ("IR ENGINE", juce::dontSendNotification);

    room_size_label_.setText ("Room Size", juce::dontSendNotification);
    room_size_slider_.setRange ({0.1f, 2.f}, 0);
    room_size_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    room_size_slider_.onDragEnd = [&]
    {
        ir_engine_parameter_context.dispatch (
            UpdateRoomSize (static_cast<float> (room_size_slider_.getValue ())));
    };

    reverb_time_label_.setText ("Reverb Time", juce::dontSendNotification);
    reverb_time_slider_.setRange ({0.f, 1.f}, 0);
    reverb_time_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    reverb_time_slider_.onDragEnd = [&]
    {
        ir_engine_parameter_context.dispatch (
            UpdateReverbTime (static_cast<float> (reverb_time_slider_.getValue ())));
    };
}

void IrEnginePanel::resized ()
{
    auto room_size_layout = LookAndFeel::SliderLabelLayout (room_size_slider_, room_size_label_);
    auto reverb_time_layout =
        LookAndFeel::SliderLabelLayout (reverb_time_slider_, reverb_time_label_);

    juce::FlexBox parameter_layout;
    parameter_layout.flexDirection = juce::FlexBox::Direction::row;

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
