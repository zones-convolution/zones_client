#pragma once

#include "DraggableOrientation.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class Graph3DComponent
    : public juce::Component
    , public juce::OpenGLRenderer
{
public:
    explicit Graph3DComponent ();
    ~Graph3DComponent () override;

    void newOpenGLContextCreated () override;
    void openGLContextClosing () override;
    void renderOpenGL () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

private:
    void CreateShaders ();
    void UpdateShaders ();

    static const std::filesystem::path kShaderDirectory;

private:
    std::atomic<float> offset_x_;
    std::atomic<float> offset_y_;
    std::atomic<float> scale_;

    float rot_x_smooth_ = 0.f;
    float rot_y_smooth_ = 0.f;

    juce::OpenGLContext open_gl_context_;
    GLuint graph_texture_id_;

    std::unique_ptr<VertexBuffer> vertex_buffer_;
    std::unique_ptr<IndexBuffer> index_buffer_graph_;
    std::unique_ptr<IndexBuffer> index_buffer_grid_;
    std::unique_ptr<VertexArray> vertex_array_;

    juce::SpinLock shader_mutex_;

public:
    void mouseDown (const juce::MouseEvent & event) override;
    void mouseDrag (const juce::MouseEvent & event) override;

private:
    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_texture_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_vertex_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_graph_texture_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_colour_;

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};
    juce::Slider scale_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_x_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_y_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                   juce::Slider::TextEntryBoxPosition::NoTextBox};
    DraggableOrientation draggable_orientation_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph3DComponent)
};