#pragma once

#include "RingBuffer.h"
#include "zones_look_and_feel/LookAndFeel.h"

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

private:
    void CreateShaders ();

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

    juce::OpenGLContext open_gl_context_;
    GLuint vbo_, vao_, ebo_;

    std::unique_ptr<juce::OpenGLShaderProgram> shader;
    std::unique_ptr<Uniforms> uniforms;

    RingBuffer<GLfloat> * ring_buffer_;
    juce::AudioBuffer<GLfloat> read_buffer_;
    GLfloat visualization_buffer_ [kRingBufferReadSize];

    juce::Label status_label_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramVisualiserComponent)
};