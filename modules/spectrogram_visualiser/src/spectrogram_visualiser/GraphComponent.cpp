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

    open_gl_context_.setRenderer (this);
    open_gl_context_.attachTo (*this);

    addAndMakeVisible (status_label_);
    status_label_.setJustificationType (juce::Justification::topLeft);

    addAndMakeVisible (refresh_button_);
    refresh_button_.onClick = [&] () { UpdateShaders (); };
    UpdateShaders ();

    open_gl_context_.setContinuousRepainting (true);

    addAndMakeVisible (scale_slider_);
    scale_slider_.setRange ({0.f, 1.f}, 0.f);
    scale_slider_.setValue (0.5f);
    scale_slider_.onValueChange = [&] ()
    { scale_x = static_cast<float> (scale_slider_.getValue ()); };

    addAndMakeVisible (offset_slider_);
    offset_slider_.setRange ({-10.f, 10.f}, 0.f);
    offset_slider_.setValue (0.f);
    offset_slider_.onValueChange = [&] ()
    { offset_x = static_cast<float> (offset_slider_.getValue ()); };
}

void GraphComponent::paint (juce::Graphics & g)
{
}

GraphComponent::~GraphComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

struct point
{
    GLfloat x;
    GLfloat y;
};

void GraphComponent::newOpenGLContextCreated ()
{
    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    point graph [2000];

    for (int i = 0; i < 2000; i++)
    {
        float x = (i - 1000.0) / 100.0;
        graph [i].x = x;
        graph [i].y = sin (x * 10.0) / (1.0 + x * x);
    }

    GLCall (open_gl_context_.extensions.glGenBuffers (1, &vbo_));
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vbo_));
    GLCall (open_gl_context_.extensions.glBufferData (
        juce::gl::GL_ARRAY_BUFFER, sizeof (graph), graph, juce::gl::GL_STATIC_DRAW));
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0));
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

    GLCall (open_gl_context_.extensions.glUniform1f (uniform_offset_x_, offset_x));
    GLCall (open_gl_context_.extensions.glUniform1f (uniform_scale_x_, scale_x));

    GLCall (open_gl_context_.extensions.glGenVertexArrays (1, &vao_));
    GLCall (open_gl_context_.extensions.glBindVertexArray (vao_));

    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vbo_));

    // Setup Vertex Attributes
    GLCall (open_gl_context_.extensions.glEnableVertexAttribArray (0));
    GLCall (open_gl_context_.extensions.glVertexAttribPointer (
        0, 2, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, 0, nullptr));

    GLCall (juce::gl::glDrawArrays (juce::gl::GL_LINE_STRIP, 0, 2000));

    GLCall (open_gl_context_.extensions.glDisableVertexAttribArray (0));
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0));
}

void GraphComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (refresh_button_).withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (offset_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (scale_slider_).withHeight (20.f));

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
            uniform_offset_x_ = open_gl_context_.extensions.glGetUniformLocation (
                shader->getProgramID (), "offset_x");
            uniform_scale_x_ = open_gl_context_.extensions.glGetUniformLocation (
                shader->getProgramID (), "scale_x");
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