#pragma once

#include "model/ParameterAction.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class EditorComponent : public juce::Component
{
public:
    explicit EditorComponent (lager::context<ParameterAction> & parameter_context);
    void resized () override;

private:
    lager::context<ParameterAction> parameter_context_;

    juce::Label room_size_label_;
    juce::Slider room_size_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label reverb_time_label_;
    juce::Slider reverb_time_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label dry_wet_label_;
    juce::Slider dry_wet_mix_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
};
