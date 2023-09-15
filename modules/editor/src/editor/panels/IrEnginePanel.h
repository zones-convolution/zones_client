#pragma once

#include "model/ParameterTree.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/DividerComponent.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class IrEnginePanel : public juce::Component
{
public:
    explicit IrEnginePanel (juce::AudioProcessorValueTreeState & parameter_tree);
    void resized () override;

private:
    static const std::string kIrEnginePanelKey;

    juce::Label ir_engine_label_;
    DividerComponent top_divider_;

    juce::Label room_size_label_;
    juce::Slider room_size_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment room_size_attachment_;

    juce::Label reverb_time_label_;
    juce::Slider reverb_time_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment reverb_time_attachment_;
};
