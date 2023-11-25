#pragma once

#include "../component/DraggableOrientation.h"
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

class WaterfallRenderer : public juce::OpenGLRenderer
{
public:
    WaterfallRenderer (juce::OpenGLContext & open_gl_context,
                       DraggableOrientation & draggable_orientation);

    ~WaterfallRenderer () override = default;
    void newOpenGLContextCreated () override;
    void renderOpenGL () override;
    void openGLContextClosing () override;

    void CreateShaders ();
    void UpdateShaders ();

    std::atomic<float> offset_x_;
    std::atomic<float> offset_y_;
    std::atomic<float> scale_;

    void SetupGraphTexture (const juce::dsp::AudioBlock<const float> block);

private:
    static const std::filesystem::path kShaderDirectory;

    static constexpr size_t kVertexBufferWidth = 20;
    static constexpr size_t kVertexBufferWidthM1 = kVertexBufferWidth - 1;

    static constexpr size_t kVertexBufferHeight = 3;
    static constexpr size_t kVertexBufferHeightM1 = kVertexBufferHeight - 1;

    void SetupTexture ();
    static void CreateZeroCenteredVertexGrid (
        std::array<std::array<glm::vec2, WaterfallRenderer::kVertexBufferHeight>,
                   WaterfallRenderer::kVertexBufferWidth> & vertices);

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
    std::optional<juce::String> new_vertex_shader_;
    std::optional<juce::String> new_fragment_shader_;

    std::unique_ptr<juce::OpenGLShaderProgram> shader_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_texture_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_vertex_transform_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_graph_texture_;
    std::unique_ptr<juce::OpenGLShaderProgram::Uniform> uniform_colour_;

    std::optional<juce::Image> texture_ = std::nullopt;
};
