#pragma once

#include "../gl/IndexBuffer.h"
#include "../gl/VertexArray.h"
#include "../gl/VertexBuffer.h"
#include "Spectrogram.h"

#include <filesystem>
#include <juce_opengl/juce_opengl.h>
#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class WaterfallGraph
{
public:
    explicit WaterfallGraph (juce::OpenGLContext & open_gl_context);
    void Render (const glm::mat4 & vertex_transform, const glm::mat4 & texture_transform);
    void ContextClosing ();
    void ContextCreated ();
    void LoadTexture (const juce::Image & texture);
    void LoadShaders (const juce::String & vertex_shader, const juce::String & fragment_shader);

private:
    static constexpr size_t kVertexBufferWidth = 120;
    static constexpr size_t kVertexBufferHeight = 82;

    void DrawGrid () const;
    void DrawGraph () const;
    void UpdateShaders ();
    void UpdateTexture ();

    juce::OpenGLContext & open_gl_context_;

    GLuint graph_texture_id_ {};
    std::unique_ptr<VertexBuffer> vertex_buffer_;
    std::unique_ptr<IndexBuffer> index_buffer_graph_;
    std::unique_ptr<IndexBuffer> index_buffer_grid_;
    std::unique_ptr<VertexArray> vertex_array_;

    std::unique_ptr<juce::OpenGLShaderProgram> shader_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_texture_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_vertex_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_graph_texture_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_colour_;

    juce::SpinLock shader_mutex_;
    std::optional<juce::String> new_vertex_shader_;
    std::optional<juce::String> new_fragment_shader_;

    juce::SpinLock texture_mutex_;
    std::optional<juce::Image> new_texture_ = std::nullopt;
};