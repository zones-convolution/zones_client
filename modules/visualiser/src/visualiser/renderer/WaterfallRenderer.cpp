#include "WaterfallRenderer.h"

#include "zones_look_and_feel/LookAndFeel.h"

#if JUCE_DEBUG
const std::filesystem::path WaterfallRenderer::kShaderDirectory = SHADER_DIRECTORY;
#else
extern "C" const char shaders_graph3d_frag_glsl [];
extern "C" const unsigned shaders_graph3d_frag_glsl_size;

extern "C" const char shaders_graph3d_vert_glsl [];
extern "C" const unsigned shaders3d_graph_vert_glsl_size;
#endif

WaterfallRenderer::WaterfallRenderer (juce::OpenGLContext & open_gl_context,
                                      DraggableOrientation & draggable_orientation)
    : draggable_orientation_ (draggable_orientation)
    , open_gl_context_ (open_gl_context)
{
}

void SetupDefaultTexture (std::optional<juce::Image> & texture)
{
    texture = juce::Image (juce::Image::PixelFormat::ARGB, 1024, 1024, true);
    for (auto i = 0; i < 1024; ++i)
        for (auto j = 0; j < 1024; ++j)
            texture->setPixelAt (
                i,
                j,
                juce::Colour::fromFloatRGBA (
                    0.f, 0.f, 0.f, ((std::sin (j * 0.01f) * std::sin (i * 0.01f)) + 1.f) * 0.5f));
}

void CreateZeroCenteredVertexGrid (juce::AudioBuffer<glm::vec2> & vertices)
{
    auto width = vertices.getNumChannels ();
    auto height = vertices.getNumSamples ();

    for (auto x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            auto half_vertex_buffer_width = width / 2.f;
            auto half_vertex_buffer_height = height / 2.f;

            vertices.setSample (x,
                                y,
                                {(x - half_vertex_buffer_width) / half_vertex_buffer_width,
                                 ((y - half_vertex_buffer_height) / half_vertex_buffer_height)});
        }
    }
}

void CreateGridIndices (std::vector<GLuint> & indices, std::size_t width, std::size_t height)
{
    auto grid_index = 0;
    for (int x = 0; x < width - 1; x++)
    {
        for (int y = 0; y < height; y += 8)
        {
            indices [grid_index++] = x * height + y;
            indices [grid_index++] = (x + 1) * height + y;
        }
    }

    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width; x += 8)
        {
            indices [grid_index++] = (x * height) + y;
            indices [grid_index++] = (x * height) + y + 1;
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

void WaterfallRenderer::newOpenGLContextCreated ()
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

    CreateGridIndices (indices, kVertexBufferWidth, kVertexBufferHeight);
    index_buffer_grid_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    CreateGraphIndices (indices, kVertexBufferWidth, kVertexBufferHeight);
    index_buffer_graph_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    SetupDefaultTexture (texture_);

    GLCall (juce::gl::glActiveTexture (juce::gl::GL_TEXTURE0));
    GLCall (juce::gl::glGenTextures (1, &graph_texture_id_));

    UpdateShaders ();
}

void SaveDebugSpectrogram (juce::Image & spectrogram)
{
    juce::File spec_file (
        "/Users/LeonPS/Documents/Development/zones_client/modules/zones_look_and_feel/spec.png");
    spec_file.moveToTrash ();
    juce::FileOutputStream stream (spec_file);
    juce::PNGImageFormat png_writer;
    png_writer.writeImageToStream (spectrogram.rescaled (1920, 1080), stream);
}

juce::Image ApplyClearBorder (const juce::Image & image)
{
    auto image_with_border = image.createCopy ();
    auto width = image_with_border.getWidth ();
    auto height = image_with_border.getHeight ();
    auto clear = juce::Colour::fromRGBA (0.f, 0.f, 0.f, 0.f);

    for (int y = 0; y < height; ++y)
    {
        image_with_border.setPixelAt (0, y, clear);
        image_with_border.setPixelAt (width - 1, y, clear);
    }

    for (int x = 1; x < width - 1; ++x)
    {
        image_with_border.setPixelAt (x, 0, clear);
        image_with_border.setPixelAt (x, height - 1, clear);
    }

    return image_with_border;
}

void WaterfallRenderer::SetupGraphTexture (const juce::dsp::AudioBlock<const float> block)
{
    auto spectrogram = Spectrogram::CreateSpectrogram (block);
    auto rescaled = spectrogram.rescaled (256, 256);
    texture_ = ApplyClearBorder (rescaled);
    // SaveDebugSpectrogram (spectrogram);
}

void WaterfallRenderer::SetupTexture ()
{
    if (texture_.has_value ())
    {
        auto width = texture_->getWidth ();
        auto height = texture_->getHeight ();

        juce::AudioBuffer<GLubyte> graph {height, width};

        for (int y = 0; y < height; y++)
        {
            auto channel_pointer = graph.getWritePointer (y);
            for (int x = 0; x < width; x++)
            {
                auto pixel_value = texture_->getPixelAt (x, y).getFloatAlpha ();
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

        texture_ = std::nullopt;
    }
}

static float
Smoothed (float current_value, float target_value, float speed_factor, float delta_time)
{
    return current_value + (target_value - current_value) * speed_factor * delta_time;
}

glm::mat4 WaterfallRenderer::CreateTextureTransform ()
{
    auto scale = scale_.load ();
    auto offset_x = offset_x_.load ();
    auto offset_y = offset_y_.load ();

    return glm::translate (glm::scale (glm::mat4 (1.0f), glm::vec3 (scale, -scale, 1)),
                           glm::vec3 (offset_x, offset_y, 0));
}

glm::mat4 WaterfallRenderer::CreateVertexTransform ()
{
    rot_x_smooth_ =
        Smoothed (rot_x_smooth_, draggable_orientation_.x_rotation.load (), 4.f, 1.0f / 60.0f);
    rot_y_smooth_ =
        Smoothed (rot_y_smooth_, draggable_orientation_.y_rotation.load (), 4.f, 1.0f / 60.0f);

    auto rot_z_offset = juce::MathConstants<float>::twoPi / 8;
    auto rot_about_z =
        glm::rotate (glm::mat4 (1.f),
                     (rot_x_smooth_ * juce::MathConstants<float>::twoPi) - rot_z_offset,
                     glm::vec3 (0.f, 0.f, 1.f));
    auto rot_about_x = glm::rotate (glm::mat4 (1.f),
                                    rot_y_smooth_ * juce::MathConstants<float>::twoPi,
                                    glm::vec3 (1.f, 0.f, 0.f));
    auto rotator = rot_about_x * rot_about_z;

    auto view = glm::lookAt (
        glm::vec3 (0.f, 2.f, 1.f), glm::vec3 (0.0, 0.0, 0.0), glm::vec3 (0.0, 0.0, 1.0));

    auto projection = glm::perspective (45.f, 1920.f / 1080.f, .1f, 10.f);

    auto vertex_scale = glm::scale (glm::mat4 (1.0f), glm::vec3 (1, 0.8, 1));
    return projection * view * rotator * vertex_scale;
}

void WaterfallRenderer::DrawGraph ()
{
    index_buffer_graph_->Bind ();
    uniform_colour_->set (1.f, 1.f, 1.f, 1.f);
    GLCall (juce::gl::glDrawElements (juce::gl::GL_TRIANGLES,
                                      (kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_graph_->Unbind ();
}

void WaterfallRenderer::DrawGrid ()
{
    index_buffer_grid_->Bind ();
    uniform_colour_->set (4.f, 4.f, 4.f, 1.f);
    GLCall (juce::gl::glDrawElements (juce::gl::GL_LINES,
                                      (kVertexBufferWidth - 1) * (kVertexBufferHeight - 1) * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_grid_->Unbind ();
}

void WaterfallRenderer::renderOpenGL ()
{
    jassert (juce::OpenGLHelpers::isContextActive ());
    juce::OpenGLHelpers::clear (
        juce::LookAndFeel::getDefaultLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    GLCall (juce::gl::glEnable (juce::gl::GL_DEPTH_TEST));
    GLCall (juce::gl::glClear (juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT));
    GLCall (juce::gl::glPolygonOffset (1, 0));
    GLCall (juce::gl::glEnable (juce::gl::GL_POLYGON_OFFSET_FILL));

    SetupTexture ();
    CreateShaders ();
    shader_->use ();

    uniform_graph_texture_->set (0);
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));

    auto vertex_transform = CreateVertexTransform ();
    uniform_vertex_transform_->setMatrix4 (
        glm::value_ptr (vertex_transform), 1, juce::gl::GL_FALSE);

    auto texture_transform = CreateTextureTransform ();
    uniform_texture_transform_->setMatrix4 (
        glm::value_ptr (texture_transform), 1, juce::gl::GL_FALSE);

    vertex_array_->Bind ();
    DrawGraph ();
    DrawGrid ();
    vertex_array_->Unbind ();
}

void WaterfallRenderer::openGLContextClosing ()
{
    vertex_buffer_.reset ();
    index_buffer_graph_.reset ();
    index_buffer_grid_.reset ();
    vertex_array_.reset ();

    shader_.reset ();
    uniform_texture_transform_.reset ();
    uniform_vertex_transform_.reset ();
    uniform_graph_texture_.reset ();
    uniform_colour_.reset ();
}

void WaterfallRenderer::CreateShaders ()
{
    juce::SpinLock::ScopedTryLockType lock (shader_mutex_);
    if (! lock.isLocked ())
        return;

    if (new_vertex_shader_.has_value () && new_fragment_shader_.has_value ())
    {
        auto gl_shader_program = std::make_unique<juce::OpenGLShaderProgram> (open_gl_context_);
        juce::String status_text;

        if (gl_shader_program->addVertexShader (*new_vertex_shader_) &&
            gl_shader_program->addFragmentShader (*new_fragment_shader_) &&
            gl_shader_program->link ())
        {
            shader_.reset (gl_shader_program.release ());

            status_text =
                "GLSL: v" + juce::String (juce::OpenGLShaderProgram::getLanguageVersion (), 2);
            uniform_texture_transform_ = std::make_unique<juce::OpenGLShaderProgram::Uniform> (
                *shader_, "texture_transform");
            uniform_vertex_transform_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader_, "vertex_transform");
            uniform_graph_texture_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader_, "graph_texture");
            uniform_colour_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader_, "colour");
        }
        else
        {
            status_text = gl_shader_program->getLastError ();
        }

        new_vertex_shader_ = std::nullopt;
        new_fragment_shader_ = std::nullopt;
    }
}

void WaterfallRenderer::UpdateShaders ()
{
    juce::SpinLock::ScopedLockType lock (shader_mutex_);

#if JUCE_DEBUG
    static const auto fragment_shader_filepath = kShaderDirectory / "graph3d.frag.glsl";
    static const auto vertex_shader_filepath = kShaderDirectory / "graph3d.vert.glsl";

    auto fragment_shader_file = juce::File (fragment_shader_filepath.string ());
    auto vertex_shader_file = juce::File (vertex_shader_filepath.string ());

    new_fragment_shader_ = fragment_shader_file.loadFileAsString ();
    new_vertex_shader_ = vertex_shader_file.loadFileAsString ();
#else
    new_fragment_shader_ = shaders_graph3d_frag_glsl;
    new_vertex_shader_ = shaders_graph3d_vert_glsl;
#endif
}
