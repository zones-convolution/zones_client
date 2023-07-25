#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class GraphComponent
    : public juce::Component
    , public juce::OpenGLRenderer
{
public:
    explicit GraphComponent ();
    ~GraphComponent () override;

    void newOpenGLContextCreated () override;
    void openGLContextClosing () override;
    void renderOpenGL () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

private:
    void CreateShaders ();
    void UpdateShaders ();

    static const std::filesystem::path kShaderDirectory;

    std::atomic<float> offset_x_, scale_x_;

    juce::OpenGLContext open_gl_context_;
    std::unique_ptr<VertexBuffer> vb_;
    std::unique_ptr<VertexArray> va_;
    GLuint graph_texture_id_;

    juce::SpinLock shader_mutex_;
    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_offset_x_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_scale_x_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_mytexture_;

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};
    juce::Slider scale_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Slider offset_slider_ {juce::Slider::SliderStyle::LinearHorizontal,
                                 juce::Slider::TextEntryBoxPosition::NoTextBox};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphComponent)
};