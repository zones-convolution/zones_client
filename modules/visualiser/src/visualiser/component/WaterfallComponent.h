#pragma once

#include "../renderer/WaterfallRenderer.h"
#include "DraggableOrientation.h"
#include "look_and_feel/LookAndFeel.h"

#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class WaterfallComponent : public juce::Component
{
public:
    explicit WaterfallComponent (juce::ThreadPool & thread_pool);
    ~WaterfallComponent () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

    void SetAudioBlock (const immer::box<juce::AudioBuffer<float>> & boxed_buffer);
    void EnableDebugControls (bool enabled);

private:
    void mouseDown (const juce::MouseEvent & event) override;
    void mouseDrag (const juce::MouseEvent & event) override;

    void SetupOpenGl ();
    void ParametersUpdated ();

    DraggableOrientation draggable_orientation_;

    juce::ThreadPool & thread_pool_;
    juce::OpenGLContext open_gl_context_;
    WaterfallRenderer waterfall_renderer_ {open_gl_context_, draggable_orientation_, thread_pool_};

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh Shaders"};
    juce::Slider scale_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_x_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_y_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};

    juce::ToggleButton show_time_grid_ {"Time Grid"};
    juce::ToggleButton show_frequency_grid_ {"Frequency Grid"};
    juce::ComboBox colour_scheme_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterfallComponent)
};
