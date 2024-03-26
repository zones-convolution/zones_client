#include "WaterfallGraph.h"

WaterfallGraph::WaterfallGraph (juce::OpenGLContext & open_gl_context,
                                DynamicShaderLoader & graph_shader_loader,
                                DynamicShaderLoader & grid_shader_loader)
    : open_gl_context_ (open_gl_context)
    , graph_shader_loader_ (graph_shader_loader)
    , grid_shader_loader_ (grid_shader_loader)
{
}

void CreateZeroCenteredVertexGrid (juce::AudioBuffer<glm::vec2> & vertices)
{
    auto width = vertices.getNumChannels ();
    auto height = vertices.getNumSamples ();

    for (auto x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            auto mapped_x = juce::jmap (
                static_cast<float> (x), 0.f, static_cast<float> (width) - 1.f, -1.f, 1.f);
            auto mapped_y = juce::jmap (
                static_cast<float> (y), 0.f, static_cast<float> (height) - 1.f, -1.f, 1.f);
            vertices.setSample (x, y, {mapped_x, mapped_y});
        }
    }
}

void CreateGridIndices (std::vector<GLuint> & indices,
                        std::size_t width,
                        std::size_t height,
                        bool draw_time_grid,
                        bool draw_frequency_grid)
{
    auto grid_index = 0;

    if (draw_time_grid)
    {
        for (int x = 0; x < width - 1; x++)
        {
            for (int y = 0; y < height; y += 9)
            {
                indices [grid_index++] = x * height + y;
                indices [grid_index++] = (x + 1) * height + y;
            }
        }
    }

    if (draw_frequency_grid)
    {
        for (int y = 0; y < height - 1; y++)
        {
            for (int x = 0; x < width; x += 7)
            {
                indices [grid_index++] = (x * height) + y;
                indices [grid_index++] = (x * height) + y + 1;
            }
        }
    }
}

void CreateGraphIndices (std::vector<GLuint> & indices, std::size_t width, std::size_t height)
{
    auto graph_index = 0;
    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            auto top_left = (x * height) + y;
            auto bottom_left = top_left + 1;

            auto top_right = (x + 1) * height + y;
            auto bottom_right = top_right + 1;

            indices [graph_index++] = top_left;
            indices [graph_index++] = top_right;
            indices [graph_index++] = bottom_right;

            indices [graph_index++] = bottom_right;
            indices [graph_index++] = bottom_left;
            indices [graph_index++] = top_left;
        }
    }
}

void WaterfallGraph::ContextCreated ()
{
    juce::AudioBuffer<glm::vec2> vertices {kVertexBufferWidth, kVertexBufferHeight};
    CreateZeroCenteredVertexGrid (vertices);
    vertex_buffer_ = std::make_unique<VertexBuffer> (
        vertices.getReadPointer (0), kVertexBufferWidth * kVertexBufferHeight * sizeof (glm::vec2));

    vertex_array_ = std::make_unique<VertexArray> ();
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<GLfloat> (2);
    vertex_array_->AddBuffer (*vertex_buffer_, vertex_buffer_layout);

    std::vector<GLuint> indices;
    indices.resize ((kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6);
    CreateGraphIndices (indices, kVertexBufferWidth, kVertexBufferHeight);
    index_buffer_graph_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    parameters_ = last_parameters_;

    GLCall (juce::gl::glGenTextures (1, &graph_texture_id_));
    GLCall (juce::gl::glGenTextures (1, &colourmap_texture_id_));
}

void WaterfallGraph::SetColourMap (tinycolormap::ColormapType colour_scheme) const
{
    struct RGB
    {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    };

    static constexpr std::size_t kNumColourSamples = 80;
    std::array<RGB, kNumColourSamples> sampled_colourmap {};
    for (auto i = 0; i < kNumColourSamples; ++i)
    {
        auto colour = tinycolormap::GetColor (
            static_cast<float> (i) / static_cast<float> (kNumColourSamples), colour_scheme);
        sampled_colourmap [i] = RGB {.r = colour.ri (), .b = colour.bi (), .g = colour.gi ()};
    }

    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_1D, colourmap_texture_id_));
    GLCall (juce::gl::glTexImage1D (juce::gl::GL_TEXTURE_1D,
                                    0,
                                    juce::gl::GL_RGB,
                                    kNumColourSamples,
                                    0,
                                    juce::gl::GL_RGB,
                                    juce::gl::GL_UNSIGNED_BYTE,
                                    sampled_colourmap.data ()));

    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_1D, juce::gl::GL_TEXTURE_WRAP_S, juce::gl::GL_CLAMP_TO_EDGE);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_1D, juce::gl::GL_TEXTURE_WRAP_T, juce::gl::GL_CLAMP_TO_EDGE);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_1D, juce::gl::GL_TEXTURE_MIN_FILTER, juce::gl::GL_LINEAR);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_1D, juce::gl::GL_TEXTURE_MAG_FILTER, juce::gl::GL_LINEAR);
}

void WaterfallGraph::LoadTexture (const juce::Image & texture)
{
    juce::SpinLock::ScopedLockType lock (texture_mutex_);
    new_texture_ = texture;
}

void WaterfallGraph::UpdateTexture ()
{
    juce::SpinLock::ScopedTryLockType lock (texture_mutex_);
    if (! lock.isLocked ())
        return;

    if (new_texture_.has_value ())
    {
        auto width = new_texture_->getWidth ();
        auto height = new_texture_->getHeight ();

        juce::AudioBuffer<GLubyte> graph {height, width};

        for (int y = 0; y < height; y++)
        {
            auto channel_pointer = graph.getWritePointer (y);
            for (int x = 0; x < width; x++)
            {
                auto pixel_value = new_texture_->getPixelAt (x, y).getFloatAlpha ();
                channel_pointer [x] = static_cast<GLubyte> (std::roundf (pixel_value * 255));
            }
        }

        GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));
        GLCall (juce::gl::glTexImage2D (juce::gl::GL_TEXTURE_2D,
                                        0,
                                        juce::gl::GL_RED,
                                        width,
                                        height,
                                        0,
                                        juce::gl::GL_RED,
                                        juce::gl::GL_UNSIGNED_BYTE,
                                        graph.getReadPointer (0)));
        juce::gl::glTexParameteri (
            juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_S, juce::gl::GL_CLAMP_TO_EDGE);
        juce::gl::glTexParameteri (
            juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_T, juce::gl::GL_CLAMP_TO_EDGE);
        juce::gl::glTexParameteri (
            juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MIN_FILTER, juce::gl::GL_LINEAR);
        juce::gl::glTexParameteri (
            juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MAG_FILTER, juce::gl::GL_LINEAR);

        new_texture_ = std::nullopt;
    }
}

void WaterfallGraph::LoadParameters (const Parameters & parameters)
{
    juce::SpinLock::ScopedLockType lock (parameter_mutex_);
    parameters_ = parameters;
    last_parameters_ = parameters;
}

void WaterfallGraph::UpdateParameters ()
{
    juce::SpinLock::ScopedTryLockType lock (parameter_mutex_);
    if (! lock.isLocked ())
        return;

    if (parameters_.has_value ())
    {
        std::vector<GLuint> indices;
        indices.resize ((kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6);

        CreateGridIndices (indices,
                           kVertexBufferWidth,
                           kVertexBufferHeight,
                           parameters_->draw_time_grid,
                           parameters_->draw_frequency_grid);
        index_buffer_grid_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

        SetColourMap (parameters_->colour_scheme);

        parameters_ = std::nullopt;
    }
}

void WaterfallGraph::DrawGraph () const
{
    index_buffer_graph_->Bind ();
    GLCall (juce::gl::glDrawElements (juce::gl::GL_TRIANGLES,
                                      (kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_graph_->Unbind ();
}

void WaterfallGraph::DrawGrid () const
{
    index_buffer_grid_->Bind ();
    GLCall (juce::gl::glDrawElements (juce::gl::GL_LINES,
                                      (kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_grid_->Unbind ();
}

void SetGraphUniforms (juce::OpenGLShaderProgram & graph_shader,
                       const glm::mat4 & vertex_transform,
                       const glm::mat4 & texture_transform)
{
    graph_shader.setUniform ("graph_texture", 0);
    graph_shader.setUniform ("colourmap", 1);
    graph_shader.setUniformMat4 (
        "vertex_transform", glm::value_ptr (vertex_transform), 1, juce::gl::GL_FALSE);
    graph_shader.setUniformMat4 (
        "texture_transform", glm::value_ptr (texture_transform), 1, juce::gl::GL_FALSE);
}

void SetGridUniforms (juce::OpenGLShaderProgram & grid_shader,
                      const glm::mat4 & vertex_transform,
                      const glm::mat4 & texture_transform)
{
    grid_shader.setUniform ("graph_texture", 0);
    grid_shader.setUniformMat4 (
        "vertex_transform", glm::value_ptr (vertex_transform), 1, juce::gl::GL_FALSE);
    grid_shader.setUniformMat4 (
        "texture_transform", glm::value_ptr (texture_transform), 1, juce::gl::GL_FALSE);
    grid_shader.setUniform ("colour", 1.f, 1.f, 1.f, 1.f);
}

void WaterfallGraph::Render (const glm::mat4 & vertex_transform,
                             const glm::mat4 & texture_transform)
{
    GLCall (juce::gl::glActiveTexture (juce::gl::GL_TEXTURE0));
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));

    GLCall (juce::gl::glActiveTexture (juce::gl::GL_TEXTURE1));
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_1D, colourmap_texture_id_));

    UpdateParameters ();
    UpdateTexture ();

    vertex_array_->Bind ();

    graph_shader_loader_.Update (graph_shader_);
    graph_shader_.use ();
    SetGraphUniforms (graph_shader_, vertex_transform, texture_transform);
    DrawGraph ();

    grid_shader_loader_.Update (grid_shader_);
    grid_shader_.use ();
    SetGridUniforms (grid_shader_, vertex_transform, texture_transform);
    DrawGrid ();

    vertex_array_->Unbind ();
}

void WaterfallGraph::ContextClosing ()
{
    vertex_buffer_.reset ();
    index_buffer_graph_.reset ();
    index_buffer_grid_.reset ();
    vertex_array_.reset ();
    graph_shader_.release ();
    grid_shader_.release ();
}