#pragma once
#include "model/ParameterAction.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/DividerComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class IrEnginePanel : public juce::Component
{
public:
    IrEnginePanel (lager::context<IrEngineParameterAction> & ir_engine_parameter_context);
    void resized () override;

private:
    juce::Label ir_engine_label_;
    DividerComponent top_divider_;

    juce::Label room_size_label_;
    juce::Slider room_size_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label reverb_time_label_;
    juce::Slider reverb_time_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
};
