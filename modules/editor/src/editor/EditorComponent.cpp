#include "EditorComponent.h"

EditorComponent::EditorComponent (lager::context<ParameterAction> & parameter_context)
    : parameter_context_ (parameter_context)
{
    addAndMakeVisible (room_size_slider_);
    addAndMakeVisible (room_size_label_);
    addAndMakeVisible (reverb_time_slider_);
    addAndMakeVisible (reverb_time_label_);
    addAndMakeVisible (dry_wet_mix_slider_);
    addAndMakeVisible (dry_wet_label_);

    room_size_label_.setText ("Room Size", juce::dontSendNotification);
    room_size_slider_.setRange ({0.1f, 2.f}, 0);
    room_size_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    room_size_slider_.onDragEnd = [&]
    {
        parameter_context.dispatch (
            SetRoomSizeAction {.room_size = (float) room_size_slider_.getValue ()});
    };

    reverb_time_label_.setText ("Reverb Time", juce::dontSendNotification);
    reverb_time_slider_.setRange ({0.f, 1.f}, 0);
    reverb_time_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    reverb_time_slider_.onDragEnd = [&]
    {
        parameter_context.dispatch (
            SetReverbTimeAction {.reverb_time = (float) reverb_time_slider_.getValue ()});
    };

    dry_wet_label_.setText ("Dry/Wet", juce::dontSendNotification);
    dry_wet_mix_slider_.setRange ({0.f, 1.f}, 0);
    dry_wet_mix_slider_.setPopupDisplayEnabled (true, true, getTopLevelComponent ());
    dry_wet_mix_slider_.onValueChange = [&]
    {
        parameter_context.dispatch (
            SetDryWetMixAction {.dry_wet_mix = (float) dry_wet_mix_slider_.getValue ()});
    };
}
void EditorComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (room_size_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (room_size_slider_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (reverb_time_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (reverb_time_slider_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (dry_wet_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (dry_wet_mix_slider_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
