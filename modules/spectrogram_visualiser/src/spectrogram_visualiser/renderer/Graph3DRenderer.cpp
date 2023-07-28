#include "Graph3DRenderer.h"

#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_dsp/juce_dsp.h>
#include <tinycolormap.hpp>

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

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

juce::Image CreateGraphData ()
{
    juce::AudioFormatManager audio_format_manager;
    audio_format_manager.registerBasicFormats ();

    // auto ir_file = juce::File (kTestAudioPath.string ());
    auto ir_file = juce::File ();

    juce::AudioBuffer<float> audio_buffer;
    std::unique_ptr<juce::AudioFormatReader> reader (
        audio_format_manager.createReaderFor (ir_file));
    auto sample_rate = reader->sampleRate;
    auto bit_depth = reader->bitsPerSample;
    audio_buffer.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&audio_buffer, 0, reader->lengthInSamples, 0, true, true);

    static constexpr auto kFFTOrder = 10;
    static constexpr auto kFFTSize = 1 << kFFTOrder;

    std::array<float, kFFTSize * 2> fft_data {};
    juce::dsp::FFT fft {kFFTOrder};
    juce::dsp::WindowingFunction<float> window {kFFTSize,
                                                juce::dsp::WindowingFunction<float>::hann};

    juce::dsp::AudioBlock<float> audio_block {audio_buffer};

    auto hop = kFFTSize / 2;
    auto num_hops = 1 + (audio_block.getNumSamples () - kFFTSize) / hop;
    juce::Image spectrogram {juce::Image::RGB, (int) num_hops, kFFTSize, true};

    for (auto i = 0; i < audio_buffer.getNumSamples () - kFFTSize; i += hop)
    {
        auto sub_block = audio_block.getSubBlock (i, kFFTSize);
        std::fill (fft_data.begin (), fft_data.end (), 0.0f);
        for (auto channel = 0; channel < audio_buffer.getNumChannels (); ++channel)
        {
            auto read = sub_block.getChannelPointer (channel);
            juce::FloatVectorOperations::add (fft_data.data (), read, kFFTSize);
        }
        window.multiplyWithWindowingTable (fft_data.data (), kFFTSize);
        fft.performFrequencyOnlyForwardTransform (fft_data.data (), true);

        auto right_hand_edge = spectrogram.getWidth () - 1;
        auto image_height = spectrogram.getHeight ();

        spectrogram.moveImageSection (0, 0, 1, 0, right_hand_edge, image_height);

        auto max_level =
            juce::FloatVectorOperations::findMinAndMax (fft_data.data (), kFFTSize / 2);

        for (auto y = 1; y < image_height; ++y)
        {
            auto y_prop = 1.0f - ((float) y / (float) image_height);
            auto skewedProportionY = std::exp (std::log (0.2f * y_prop));
            auto fftDataIndex =
                (size_t) juce::jlimit (0, kFFTSize / 2, (int) (skewedProportionY * kFFTSize / 2));
            auto level = juce::jmap (
                fft_data [fftDataIndex], 0.0f, juce::jmax (max_level.getEnd (), 1e-5f), 0.0f, 1.0f);

            auto colour = tinycolormap::GetColor (level, tinycolormap::ColormapType::Viridis);
            spectrogram.setPixelAt (
                right_hand_edge,
                y,
                juce::Colour::fromFloatRGBA (colour.r (), colour.g (), colour.b (), level));
        }
    }

    return spectrogram.rescaled (1024, 1024);
}

static constexpr size_t kVertexBufferSize = 110;
static constexpr size_t kVertexBufferSizeM1 = kVertexBufferSize - 1;

void Graph3DRenderer::newOpenGLContextCreated ()
{
    //    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    //    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    //    static constexpr auto kGraphSize = 1024;
    //    auto graph = std::make_unique<std::array<std::array<GLubyte, kGraphSize>, kGraphSize>> ();
    //
    //    for (int i = 0; i < kGraphSize; i++)
    //    {
    //        for (int j = 0; j < kGraphSize; j++)
    //        {
    //            float x = (i - kGraphSize / 2) / (kGraphSize / 2.0);
    //            float y = (j - kGraphSize / 2) / (kGraphSize / 2.0);
    //            float z = sinf (y * M_PI * 2.f) * sinf (x * M_PI * 2.f);
    //            (*graph) [i][j] = roundf (z * 127 + 128);
    //        }
    //    }

    //    auto spectrogram = CreateGraphData ();
    //
    //    juce::ImageConvolutionKernel convolution_kernel {4};
    //    convolution_kernel.createGaussianBlur (4.f);
    //    convolution_kernel.applyToImage (spectrogram, spectrogram, spectrogram.getBounds ());

    //    auto path = kTestAudioDirectory / "test_audio.png";
    //    auto file = juce::File (path.string ());
    //    file.moveToTrash ();
    //    juce::FileOutputStream stream (file);
    //    juce::PNGImageFormat pngWriter;
    //    pngWriter.writeImageToStream (spectrogram, stream);

    //    auto width = spectrogram.getWidth ();
    //    auto height = spectrogram.getHeight ();

    auto width = 1024;
    auto height = 1024;
    juce::AudioBuffer<GLubyte> graph {width, height};

    for (int x = 0; x < width; x++)
    {
        auto channel_pointer = graph.getWritePointer (x);
        for (int y = 0; y < height; y++)
        {
            float x_n = (x - width / 2) / (width / 2.0);
            float y_n = (y - height / 2) / (height / 2.0);
            float z = sinf (y_n * M_PI * 2.f) * sinf (x_n * M_PI * 2.f);
            channel_pointer [y] = roundf (z * 127 + 128);

            //            auto pixel_value = spectrogram.getPixelAt (x, y).getFloatAlpha ();
            //            channel_pointer [y] = roundf (pixel_value * 127 + 128);
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

    CreateShaders ();
    shader_->use ();

    rot_x_smooth_ =
        Smoothed (rot_x_smooth_, draggable_orientation_.x_rotation.load (), 4.f, 1.0f / 60.0f);
    rot_y_smooth_ =
        Smoothed (rot_y_smooth_, draggable_orientation_.y_rotation.load (), 4.f, 1.0f / 60.0f);

    auto rot_about_z = glm::rotate (glm::mat4 (1.f),
                                    rot_x_smooth_ * juce::MathConstants<float>::twoPi,
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
        glm::translate (glm::scale (glm::mat4 (1.0f), glm::vec3 (-scale, scale, 1)),
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
    index_buffer_graph_->Bind ();
    uniform_colour_->set (0.f, 0.f, 0.f, 1.f);
    GLCall (juce::gl::glDrawElements (juce::gl::GL_LINES,
                                      kVertexBufferSizeM1 * kVertexBufferSizeM1 * 6,
                                      juce::gl::GL_UNSIGNED_INT,
                                      0));
    index_buffer_grid_->Unbind ();
    vertex_array_->Unbind ();
}

void Graph3DRenderer::openGLContextClosing ()
{
    shader_.reset ();
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