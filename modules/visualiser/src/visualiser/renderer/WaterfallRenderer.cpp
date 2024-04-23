#include "WaterfallRenderer.h"

#include "look_and_feel/LookAndFeel.h"

#include <utility>

const std::filesystem::path WaterfallRenderer::kShaderDirectory = SHADER_DIRECTORY;
extern "C" const char shaders_graph3d_frag_glsl [];
extern "C" const unsigned shaders_graph3d_frag_glsl_size;

extern "C" const char shaders_graph3d_vert_glsl [];
extern "C" const unsigned shaders_graph3d_vert_glsl_size;

extern "C" const char shaders_grid3d_frag_glsl [];
extern "C" const unsigned shaders_grid3d_frag_glsl_size;

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

CreateTextureJob::CreateTextureJob (const immer::box<juce::AudioBuffer<float>> & boxed_buffer,
                                    std::function<void (const juce::Image & texture)> callback)
    : ThreadPoolJob ("create_texture_job")
    , boxed_buffer_ (boxed_buffer)
    , callback_ (std::move (callback))
{
}

juce::ThreadPoolJob::JobStatus CreateTextureJob::runJob ()
{
    if (shouldExit ())
        return jobHasFinished;

    juce::dsp::AudioBlock<const float> block {boxed_buffer_.get ()};
    if (block.getNumChannels () == 0 || block.getNumSamples () == 0)
        return jobHasFinished;

    auto spectrogram = Spectrogram::CreateSpectrogram (block);
    if (shouldExit ())
        return jobHasFinished;

    auto rescaled = spectrogram.rescaled (256, 256);
    if (shouldExit ())
        return jobHasFinished;

    auto with_clear_border = ApplyClearBorder (rescaled);
    if (shouldExit ())
        return jobHasFinished;

    // SaveDebugSpectrogram (with_clear_border);
    // if (shouldExit ())
    //     return jobHasFinished;

    callback_ (with_clear_border);
    return jobHasFinished;
}

WaterfallRenderer::WaterfallRenderer (juce::OpenGLContext & open_gl_context,
                                      DraggableOrientation & draggable_orientation,
                                      juce::ThreadPool & thread_pool)
    : draggable_orientation_ (draggable_orientation)
    , open_gl_context_ (open_gl_context)
    , graph_shader_loader_ (
          kShaderDirectory,
          DynamicShaderLoader::ShaderLoader {.shader_file = "graph3d.vert.glsl",
                                             .shader_value = shaders_graph3d_vert_glsl},
          DynamicShaderLoader::ShaderLoader {.shader_file = "graph3d.frag.glsl",
                                             .shader_value = shaders_graph3d_frag_glsl})
    , grid_shader_loader_ (
          kShaderDirectory,
          DynamicShaderLoader::ShaderLoader {.shader_file = "graph3d.vert.glsl",
                                             .shader_value = shaders_graph3d_vert_glsl},
          DynamicShaderLoader::ShaderLoader {.shader_file = "grid3d.frag.glsl",
                                             .shader_value = shaders_grid3d_frag_glsl})
    , thread_pool_ (thread_pool)
{
}

juce::Image GenerateDefaultTexture ()
{
    juce::Image default_texture {juce::Image::PixelFormat::ARGB, 1024, 1024, true};
    for (auto i = 0; i < 1024; ++i)
        for (auto j = 0; j < 1024; ++j)
            default_texture.setPixelAt (
                i,
                j,
                juce::Colour::fromFloatRGBA (
                    0.f, 0.f, 0.f, ((std::sin (j * 0.01f) * std::sin (i * 0.01f)) + 1.f) * 0.5f));
    return default_texture;
}

void WaterfallRenderer::newOpenGLContextCreated ()
{
    waterfall_graph_.ContextCreated ();
    waterfall_graph_.LoadTexture (GenerateDefaultTexture ());
    UpdateShaders ();
}

void SaveDebugSpectrogram (juce::Image & spectrogram)
{
    juce::File spec_file (
        "/Users/LeonPS/Documents/Development/zones_client/modules/zones_look_and_feel/spec.png");
    spec_file.moveToTrash ();
    juce::FileOutputStream stream (spec_file);
    juce::PNGImageFormat png_writer;
    png_writer.writeImageToStream (spectrogram.rescaled (256, 256), stream);
}

void WaterfallRenderer::SetupGraphTexture (
    const immer::box<juce::AudioBuffer<float>> & boxed_buffer)
{
    thread_pool_.removeJob (last_texture_job_, true, 0);
    last_texture_job_ = new CreateTextureJob (boxed_buffer,
                                              [&] (const juce::Image & texture)
                                              {
                                                  std::lock_guard lock {graph_mutex_};
                                                  waterfall_graph_.LoadTexture (texture);
                                              });
    thread_pool_.addJob (last_texture_job_, true);
}

static float
Smoothed (float current_value, float target_value, float speed_factor, float delta_time)
{
    return current_value + (target_value - current_value) * speed_factor * delta_time;
}

glm::mat4 WaterfallRenderer::CreateTextureTransform () const
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

    auto vertex_scale =
        glm::scale (glm::mat4 (1.0f), glm::vec3 (1.0, WaterfallGraph::kVertexBufferRatio, 1.0));
    return projection * view * rotator * vertex_scale;
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

    auto vertex_transform = CreateVertexTransform ();
    auto texture_transform = CreateTextureTransform ();

    std::lock_guard lock {graph_mutex_};
    waterfall_graph_.Render (vertex_transform, texture_transform);
}

void WaterfallRenderer::openGLContextClosing ()
{
    std::lock_guard lock {graph_mutex_};
    waterfall_graph_.ContextClosing ();
}

void WaterfallRenderer::UpdateShaders ()
{
    graph_shader_loader_.Load ();
    grid_shader_loader_.Load ();
}

void WaterfallRenderer::LoadParameters (const WaterfallGraph::Parameters & parameters)
{
    std::lock_guard lock {graph_mutex_};
    waterfall_graph_.LoadParameters (parameters);
}
