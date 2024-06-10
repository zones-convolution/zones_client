#pragma once
#include "look_and_feel/components/DividerComponent.h"
#include "model/ParameterTree.h"

#include <juce_gui_basics/juce_gui_basics.h>

class EQComponent : public juce::Component
{
public:
    EQComponent (juce::AudioProcessorValueTreeState & parameter_tree);
    void resized () override;

private:
    static const std::string kEQPanelKey;

    juce::Label eq_label_;
    DividerComponent divider_;

    juce::Label bass_label_;
    juce::Slider bass_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                               juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment bass_attachment_;

    juce::Label treble_label_;
    juce::Slider treble_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                 juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment treble_attachment_;
};
