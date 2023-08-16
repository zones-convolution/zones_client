#pragma once

#include "../renderer/WaterfallRenderer.h"
#include "DraggableOrientation.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class WaterfallComponent : public juce::Component
{
public:
    explicit WaterfallComponent ();
    ~WaterfallComponent () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

    void SetAudioBlock (const juce::dsp::AudioBlock<const float> audio_block);

private:
    void mouseDown (const juce::MouseEvent & event) override;
    void mouseDrag (const juce::MouseEvent & event) override;

    void SetupOpenGl ();

    DraggableOrientation draggable_orientation_;

    juce::OpenGLContext open_gl_context_;
    WaterfallRenderer waterfall_renderer_ {open_gl_context_, draggable_orientation_};

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};
    juce::Slider scale_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_x_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_y_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterfallComponent)
};