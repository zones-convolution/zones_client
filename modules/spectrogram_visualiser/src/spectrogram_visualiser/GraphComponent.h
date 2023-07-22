#pragma once

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

    juce::OpenGLContext open_gl_context_;
    GLuint vbo_, vao_;

    juce::SpinLock shader_mutex_;

    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;

    juce::Label status_label_;
    juce::TextButton refresh_button_ {"Refresh"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphComponent)
};