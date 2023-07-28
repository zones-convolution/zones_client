#pragma once

#include "../renderer/Graph3DRenderer.h"
#include "DraggableOrientation.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class Graph3DComponent : public juce::Component
{
public:
    explicit Graph3DComponent ();
    ~Graph3DComponent () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

private:
    void mouseDown (const juce::MouseEvent & event) override;
    void mouseDrag (const juce::MouseEvent & event) override;

    void SetupOpenGl ();

    DraggableOrientation draggable_orientation_;

    juce::OpenGLContext open_gl_context_;
    Graph3DRenderer graph3d_renderer_ {open_gl_context_, draggable_orientation_};

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};
    juce::Slider scale_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_x_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_y_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph3DComponent)
};