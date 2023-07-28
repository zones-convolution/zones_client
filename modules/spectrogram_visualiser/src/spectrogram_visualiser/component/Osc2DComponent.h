#pragma once

#include "../gl/IndexBuffer.h"
#include "../gl/VertexArray.h"
#include "../gl/VertexBuffer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class Osc2DComponent
    : public juce::Component
    , public juce::OpenGLRenderer
{
public:
    static constexpr int kRingBufferReadSize = 256;

    explicit Osc2DComponent ();
    ~Osc2DComponent () override;

    void Start ();
    void Stop ();

    void newOpenGLContextCreated () override;
    void openGLContextClosing () override;
    void renderOpenGL () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

private:
    void CreateShaders ();
    void UpdateShaders ();
    static const std::filesystem::path kShaderDirectory;

    juce::OpenGLContext open_gl_context_;
    std::unique_ptr<VertexBuffer> vb_;
    std::unique_ptr<IndexBuffer> ib_;
    std::unique_ptr<VertexArray> va_;

    juce::SpinLock shader_mutex_;
    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> resolution_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> audio_sample_data_;

    juce::AudioBuffer<float> read_buffer_;
    GLfloat visualization_buffer_ [kRingBufferReadSize];

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Osc2DComponent)
};