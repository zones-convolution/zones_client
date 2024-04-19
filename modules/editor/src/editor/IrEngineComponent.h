#pragma once

#include "look_and_feel/components/DividerComponent.h"
#include "model/ParameterTree.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class IrEngineComponent : public juce::Component
{
public:
    explicit IrEngineComponent (juce::AudioProcessorValueTreeState & parameter_tree);
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

    juce::Label resampler_label_;
    juce::Slider resampler_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                    juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment resampler_attachment_;
};
