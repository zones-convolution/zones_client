#pragma once

#include "../gl/DynamicShaderLoader.h"
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
#include <tinycolormap.hpp>

class WaterfallGraph
{
public:
    struct Parameters
    {
        bool draw_time_grid;
        bool draw_frequency_grid;
        tinycolormap::ColormapType colour_scheme;
    };

    explicit WaterfallGraph (juce::OpenGLContext & open_gl_context,
                             DynamicShaderLoader & graph_shader_loader,
                             DynamicShaderLoader & grid_shader_loader);
    void Render (const glm::mat4 & vertex_transform, const glm::mat4 & texture_transform);
    void ContextClosing ();
    void ContextCreated ();
    void LoadTexture (const juce::Image & texture);
    void LoadParameters (const Parameters & parameters);

private:
    static constexpr size_t kVertexBufferWidth = 120;
    static constexpr size_t kVertexBufferHeight = 82;

    void DrawGrid () const;
    void DrawGraph () const;
    void UpdateTexture ();
    void SetColourMap (tinycolormap::ColormapType colour_scheme) const;
    void UpdateParameters ();

    juce::OpenGLContext & open_gl_context_;

    GLuint colourmap_texture_id_ {};
    GLuint graph_texture_id_ {};
    std::unique_ptr<VertexBuffer> vertex_buffer_;
    std::unique_ptr<IndexBuffer> index_buffer_graph_;
    std::unique_ptr<IndexBuffer> index_buffer_grid_;
    std::unique_ptr<VertexArray> vertex_array_;

    DynamicShaderLoader & graph_shader_loader_;
    juce::OpenGLShaderProgram graph_shader_ {open_gl_context_};

    DynamicShaderLoader & grid_shader_loader_;
    juce::OpenGLShaderProgram grid_shader_ {open_gl_context_};

    juce::SpinLock texture_mutex_;
    std::optional<juce::Image> new_texture_ = std::nullopt;

    juce::SpinLock parameter_mutex_;
    std::optional<Parameters> parameters_ =
        Parameters {.draw_time_grid = true,
                    .draw_frequency_grid = true,
                    .colour_scheme = tinycolormap::ColormapType::Turbo};
};
