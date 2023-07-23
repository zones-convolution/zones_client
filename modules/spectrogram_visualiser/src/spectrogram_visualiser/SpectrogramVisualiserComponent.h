#pragma once

#include "IndexBuffer.h"
#include "RingBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class SpectrogramVisualiserComponent
    : public juce::Component
    , public juce::OpenGLRenderer
{
public:
    static constexpr int kRingBufferReadSize = 256;

    explicit SpectrogramVisualiserComponent (RingBuffer<GLfloat> * ringBuffer);
    ~SpectrogramVisualiserComponent () override;

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
    struct Uniforms
    {
        Uniforms (juce::OpenGLContext & open_gl_context,
                  juce::OpenGLShaderProgram & shader_program);
        std::unique_ptr<juce::OpenGLShaderProgram::Uniform> resolution, audio_sample_data;

    private:
        static juce::OpenGLShaderProgram::Uniform *
        CreateUniform (juce::OpenGLContext & open_gl_context,
                       juce::OpenGLShaderProgram & shader_program,
                       const char * uniform_name);
    };

    static const std::filesystem::path kShaderDirectory;

    juce::OpenGLContext open_gl_context_;
    GLuint vao_;
    std::unique_ptr<VertexBuffer> vb_;
    std::unique_ptr<IndexBuffer> ib_;
    std::unique_ptr<VertexArray> va_;

    juce::SpinLock shader_mutex_;
    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<Uniforms> uniforms;

    RingBuffer<GLfloat> * ring_buffer_;
    juce::AudioBuffer<GLfloat> read_buffer_;
    GLfloat visualization_buffer_ [kRingBufferReadSize];

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramVisualiserComponent)
};