#pragma once

#include "../component/DraggableOrientation.h"
#include "../gl/IndexBuffer.h"
#include "../gl/VertexArray.h"
#include "../gl/VertexBuffer.h"
#include "Spectrogram.h"

#include <filesystem>
#include <juce_opengl/juce_opengl.h>

class Graph3DRenderer : public juce::OpenGLRenderer
{
public:
    Graph3DRenderer (juce::OpenGLContext & open_gl_context,
                     DraggableOrientation & draggable_orientation);

    ~Graph3DRenderer () override = default;
    void newOpenGLContextCreated () override;
    void renderOpenGL () override;
    void openGLContextClosing () override;

    void CreateShaders ();
    void UpdateShaders ();

    std::atomic<float> offset_x_;
    std::atomic<float> offset_y_;
    std::atomic<float> scale_;

    void SetupGraphTexture (const juce::dsp::AudioBlock<float> block);
    
private:
    static const std::filesystem::path kShaderDirectory;

    DraggableOrientation & draggable_orientation_;
    float rot_x_smooth_ = 0.f;
    float rot_y_smooth_ = 0.f;

    juce::OpenGLContext & open_gl_context_;

    GLuint graph_texture_id_ {};
    std::unique_ptr<VertexBuffer> vertex_buffer_;
    std::unique_ptr<IndexBuffer> index_buffer_graph_;
    std::unique_ptr<IndexBuffer> index_buffer_grid_;
    std::unique_ptr<VertexArray> vertex_array_;

    juce::SpinLock shader_mutex_;
    juce::String new_vertex_shader_;
    juce::String new_fragment_shader_;
    std::unique_ptr<juce::OpenGLShaderProgram> shader_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_texture_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_vertex_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_graph_texture_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_colour_;
};
