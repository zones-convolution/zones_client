#include "GraphComponent.h"

#include "GLUtils.h"

#include <memory>

#if JUCE_DEBUG
const std::filesystem::path GraphComponent::kShaderDirectory = SHADER_DIRECTORY;
#elif
extern "C" const char shaders_graph_frag_glsl [];
extern "C" const unsigned shaders_graph_frag_glsl_size;

extern "C" const char shaders_graph_vert_glsl [];
extern "C" const unsigned shaders_graph_vert_glsl_size;
#endif

GraphComponent::GraphComponent ()
{
    setOpaque (true);
    open_gl_context_.setOpenGLVersionRequired (juce::OpenGLContext::OpenGLVersion::openGL4_1);

    if (auto * peer = getPeer ())
        peer->setCurrentRenderingEngine (0);

    open_gl_context_.setComponentPaintingEnabled (true);
    open_gl_context_.setRenderer (this);
    open_gl_context_.attachTo (*this);

    addAndMakeVisible (status_label_);
    status_label_.setJustificationType (juce::Justification::topLeft);

    addAndMakeVisible (refresh_button_);
    refresh_button_.onClick = [&] () { UpdateShaders (); };
    UpdateShaders ();

    addAndMakeVisible (scale_slider_);
    scale_slider_.setRange ({0.f, 1.f}, 0.f);
    scale_slider_.setValue (0.5f);
    scale_slider_.onValueChange = [&] ()
    { scale_x_ = static_cast<float> (scale_slider_.getValue ()); };

    addAndMakeVisible (offset_slider_);
    offset_slider_.setRange ({-10.f, 10.f}, 0.f);
    offset_slider_.setValue (0.f);
    offset_slider_.onValueChange = [&] ()
    { offset_x_ = static_cast<float> (offset_slider_.getValue ()); };
}

void GraphComponent::paint (juce::Graphics & g)
{
}

GraphComponent::~GraphComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

void GraphComponent::newOpenGLContextCreated ()
{
    int vertex_texture_units;
    GLCall (juce::gl::glGetIntegerv (juce::gl::GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                                     &vertex_texture_units));
    if (! vertex_texture_units)
        DBG ("Your graphics cards does not support texture lookups in the vertex shader!");

    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    std::array<GLfloat, 400> x_distribution {};
    for (auto i = 0; i < x_distribution.size (); i++)
        x_distribution [i] = ((float) i - 50.f) / 50.0f;

    vb_ = std::make_unique<VertexBuffer> (
        open_gl_context_, x_distribution.data (), sizeof (x_distribution));

    va_ = std::make_unique<VertexArray> (open_gl_context_);
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<GLfloat> (1);
    va_->AddBuffer (*vb_, vertex_buffer_layout);

    std::array<GLubyte, 2048> graph {};
    for (auto i = 0; i < graph.size (); i++)
    {
        float x = (i - 1024.0f) / 100.0f;
        float y = std::sin (x * 10.0f) / (1.0f + x * x);
        graph [i] = std::roundf (y * 128.0f + 128.0f);
    }

    GLCall (juce::gl::glActiveTexture (juce::gl::GL_TEXTURE0));
    GLCall (juce::gl::glGenTextures (1, &graph_texture_id_));
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));

    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MIN_FILTER, juce::gl::GL_LINEAR);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MAG_FILTER, juce::gl::GL_LINEAR);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_S, juce::gl::GL_CLAMP_TO_EDGE);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_T, juce::gl::GL_CLAMP_TO_EDGE);

    GLCall (juce::gl::glTexImage2D (juce::gl::GL_TEXTURE_2D,
                                    0,
                                    juce::gl::GL_RED,
                                    graph.size (),
                                    1,
                                    0,
                                    juce::gl::GL_RED,
                                    juce::gl::GL_UNSIGNED_BYTE,
                                    graph.data ()));
}

void GraphComponent::openGLContextClosing ()
{
    shader.reset ();
}

void GraphComponent::renderOpenGL ()
{
    jassert (juce::OpenGLHelpers::isContextActive ());
    juce::OpenGLHelpers::clear (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    CreateShaders ();
    shader->use ();
    uniform_offset_x_->set (offset_x_.load ());
    uniform_scale_x_->set (scale_x_.load ());
    uniform_mytexture_->set (0);

    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, graph_texture_id_));

    va_->Bind ();
    GLCall (juce::gl::glDrawArrays (juce::gl::GL_LINE_STRIP, 0, 101));
    va_->Unbind ();
}

void GraphComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (offset_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (scale_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (refresh_button_).withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void GraphComponent::CreateShaders ()
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
            shader = std::move (gl_shader_program);
            status_text =
                "GLSL: v" + juce::String (juce::OpenGLShaderProgram::getLanguageVersion (), 2);
            uniform_offset_x_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader, "offset_x");
            uniform_scale_x_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader, "scale_x");
            uniform_mytexture_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader, "mytexture");
        }
        else
        {
            status_text = gl_shader_program->getLastError ();
        }

        juce::MessageManager::callAsync (
            [&, status_text] ()
            { status_label_.setText (status_text, juce::dontSendNotification); });
    }
}

void GraphComponent::UpdateShaders ()
{
    juce::SpinLock::ScopedLockType lock (shader_mutex_);

#if JUCE_DEBUG
    static const auto fragment_shader_filepath = kShaderDirectory / "graph.frag.glsl";
    static const auto vertex_shader_filepath = kShaderDirectory / "graph.vert.glsl";

    auto fragment_shader_file = juce::File (fragment_shader_filepath.string ());
    auto vertex_shader_file = juce::File (vertex_shader_filepath.string ());

    new_fragment_shader_ = fragment_shader_file.loadFileAsString ();
    new_vertex_shader_ = vertex_shader_file.loadFileAsString ();
#elif
    new_fragment_shader_ = shaders_graph_frag_glsl;
    new_vertex_shader_ = shaders_graph_vert_glsl;
#endif
}