#include "Graph3DRenderer.h"

#include "zones_look_and_feel/LookAndFeel.h"

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

const std::filesystem::path kTestAudioDirectory = TEST_AUDIO_DIRECTORY;

#if JUCE_DEBUG
const std::filesystem::path Graph3DRenderer::kShaderDirectory = SHADER_DIRECTORY;
#else
extern "C" const char shaders_graph3d_frag_glsl [];
extern "C" const unsigned shaders_graph3d_frag_glsl_size;

extern "C" const char shaders_graph3d_vert_glsl [];
extern "C" const unsigned shaders3d_graph_vert_glsl_size;
#endif

Graph3DRenderer::Graph3DRenderer (juce::OpenGLContext & open_gl_context,
                                  DraggableOrientation & draggable_orientation)
    : draggable_orientation_ (draggable_orientation)
    , open_gl_context_ (open_gl_context)
{
}

static constexpr size_t kVertexBufferSize = 110;
static constexpr size_t kVertexBufferSizeM1 = kVertexBufferSize - 1;

void Graph3DRenderer::newOpenGLContextCreated ()
{
    //    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    //    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    std::array<std::array<glm::vec2, kVertexBufferSize>, kVertexBufferSize> vertices {};
    for (int i = 0; i < kVertexBufferSize; i++)
    {
        for (int j = 0; j < kVertexBufferSize; j++)
        {
            auto hvbs = (float) kVertexBufferSizeM1 / 2.f;
            vertices [i][j].x = (j - hvbs) / hvbs;
            vertices [i][j].y = (i - hvbs) / hvbs;
        }
    }
    vertex_buffer_ = std::make_unique<VertexBuffer> (vertices.data (), sizeof (vertices));

    std::array<GLuint, kVertexBufferSizeM1 * kVertexBufferSizeM1 * 6> indices {};
    int i = 0;

    for (int y = 0; y < kVertexBufferSizeM1; y++)
    {
        for (int x = 0; x < kVertexBufferSizeM1; x++)
        {
            indices [i++] = y * kVertexBufferSize + x;
            indices [i++] = y * kVertexBufferSize + x + 1;
        }
    }

    for (int x = 0; x < kVertexBufferSizeM1; x++)
    {
        for (int y = 0; y < kVertexBufferSizeM1; y++)
        {
            indices [i++] = y * kVertexBufferSize + x;
            indices [i++] = (y + 1) * kVertexBufferSize + x;
        }
    }
    index_buffer_grid_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    i = 0;
    for (int y = 0; y < kVertexBufferSizeM1; y++)
    {
        for (int x = 0; x < kVertexBufferSizeM1; x++)
        {
            indices [i++] = y * kVertexBufferSize + x;
            indices [i++] = y * kVertexBufferSize + x + 1;
            indices [i++] = (y + 1) * kVertexBufferSize + x + 1;

            indices [i++] = y * kVertexBufferSize + x;
            indices [i++] = (y + 1) * kVertexBufferSize + x + 1;
            indices [i++] = (y + 1) * kVertexBufferSize + x;
        }
    }
    index_buffer_graph_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    vertex_array_ = std::make_unique<VertexArray> ();
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<GLfloat> (2);
    vertex_array_->AddBuffer (*vertex_buffer_, vertex_buffer_layout);
}

void Graph3DRenderer::SetupGraphTexture (const juce::dsp::AudioBlock<const float> block)
{
    auto spectrogram = Spectrogram::CreateSpectrogram (block).rescaled (1024, 1024);
    texture_ = spectrogram;

    //    juce::ImageConvolutionKernel convolution_kernel {4};
    //    convolution_kernel.createGaussianBlur (4.f);
    //    convolution_kernel.applyToImage (spectrogram, spectrogram, spectrogram.getBounds ());

    //    static const auto kImagePath = kTestAudioDirectory / "result.png";
    //    auto file = juce::File (kImagePath.string ());
    //    file.moveToTrash ();
    //    juce::FileOutputStream stream (file);
    //    juce::PNGImageFormat writer;
    //    writer.writeImageToStream (spectrogram, stream);
}

void Graph3DRenderer::SetupTexture ()
{
    if (texture_.has_value ())
    {
        auto width = texture_->getWidth ();
        auto height = texture_->getHeight ();

        juce::AudioBuffer<GLubyte> graph {width, height};

        for (int x = 0; x < width; x++)
        {
            auto channel_pointer = graph.getWritePointer (x);
            for (int y = 0; y < height; y++)
            {
                auto pixel_value = texture_->getPixelAt (x, y).getFloatAlpha ();
                channel_pointer [y] = static_cast<GLubyte> (std::roundf (pixel_value * 127 + 128));
            }
        }

        GLCall (juce::gl::glActiveTexture (juce::gl::GL_TEXTURE0));
        GLCall (juce::gl::glGenTextures (1, &graph_texture_id_));

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

void Graph3DRenderer::renderOpenGL ()
{
    jassert (juce::OpenGLHelpers::isContextActive ());
    juce::OpenGLHelpers::clear (
        juce::LookAndFeel::getDefaultLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    SetupTexture ();
    CreateShaders ();
    shader_->use ();

    rot_x_smooth_ =
        Smoothed (rot_x_smooth_, draggable_orientation_.x_rotation.load (), 4.f, 1.0f / 60.0f);
    rot_y_smooth_ =
        Smoothed (rot_y_smooth_, draggable_orientation_.y_rotation.load (), 4.f, 1.0f / 60.0f);

    auto rot_about_z = glm::rotate (glm::mat4 (1.f),
                                    (rot_x_smooth_ * juce::MathConstants<float>::twoPi) -
                                        (juce::MathConstants<float>::twoPi / 8),
                                    glm::vec3 (0.f, 0.f, 1.f));
    auto rot_about_x = glm::rotate (glm::mat4 (1.f),
                                    rot_y_smooth_ * juce::MathConstants<float>::twoPi,
                                    glm::vec3 (1.f, 0.f, 0.f));
    auto rotator = rot_about_x * rot_about_z;
    auto view = glm::lookAt (
        glm::vec3 (0.f, 4.f, 2.f), glm::vec3 (0.0, 0.0, 0.0), glm::vec3 (0.0, 0.0, 1.0));
    auto projection = glm::perspective (45.0f, 1.0f * 640 / 480, 0.1f, 10.0f);
    auto vertex_transform = projection * view * rotator;
    uniform_vertex_transform_->setMatrix4 (
        glm::value_ptr (vertex_transform), 1, juce::gl::GL_FALSE);

    auto scale = scale_.load ();
    auto offset_x = offset_x_.load ();
    auto offset_y = offset_y_.load ();

    glm::mat4 texture_transform =
        glm::translate (glm::scale (glm::mat4 (1.0f), glm::vec3 (scale, -scale, 1)),
                        glm::vec3 (offset_x, offset_y, 0));
    uniform_texture_transform_->setMatrix4 (
        glm::value_ptr (texture_transform), 1, juce::gl::GL_FALSE);

    uniform_graph_texture_->set (0);
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));
    uniform_colour_->set (1.f, 1.f, 1.f, 1.f);

    vertex_array_->Bind ();
    index_buffer_graph_->Bind ();
    GLCall (juce::gl::glDrawElements (juce::gl::GL_TRIANGLES,
                                      kVertexBufferSizeM1 * kVertexBufferSizeM1 * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_graph_->Unbind ();
    //    index_buffer_graph_->Bind ();
    //    uniform_colour_->set (0.f, 0.f, 0.f, 1.f);
    //    GLCall (juce::gl::glDrawElements (juce::gl::GL_LINES,
    //                                      kVertexBufferSizeM1 * kVertexBufferSizeM1 * 6,
    //                                      juce::gl::GL_UNSIGNED_INT,
    //                                      0));
    //    index_buffer_grid_->Unbind ();
    vertex_array_->Unbind ();
}

void Graph3DRenderer::openGLContextClosing ()
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

void Graph3DRenderer::CreateShaders ()
{
    juce::SpinLock::ScopedTryLockType lock (shader_mutex_);
    if (! lock.isLocked ())
        return;

    auto gl_shader_program = std::make_unique<juce::OpenGLShaderProgram> (open_gl_context_);

    if (new_vertex_shader_.isNotEmpty () || new_fragment_shader_.isNotEmpty ())
    {
        juce::String status_text;
        if (gl_shader_program->addVertexShader (new_vertex_shader_) &&
            gl_shader_program->addFragmentShader (new_fragment_shader_) &&
            gl_shader_program->link ())
        {
            shader_ = std::move (gl_shader_program);
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

        //        juce::MessageManager::callAsync (
        //            [&, status_text] ()
        //            { status_label_.setText (status_text, juce::dontSendNotification); });
    }
}

void Graph3DRenderer::UpdateShaders ()
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
