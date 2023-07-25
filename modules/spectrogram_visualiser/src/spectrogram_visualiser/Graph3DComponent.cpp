#include "Graph3DComponent.h"

#include "GLUtils.h"

#include <memory>

#if JUCE_DEBUG
const std::filesystem::path Graph3DComponent::kShaderDirectory = SHADER_DIRECTORY;
#elif
extern "C" const char shaders_graph3d_frag_glsl [];
extern "C" const unsigned shaders_graph3d_frag_glsl_size;

extern "C" const char shaders_graph3d_vert_glsl [];
extern "C" const unsigned shaders3d_graph_vert_glsl_size;
#endif

Graph3DComponent::Graph3DComponent ()
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
}

void Graph3DComponent::paint (juce::Graphics & g)
{
}

Graph3DComponent::~Graph3DComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

struct Vec2
{
    GLfloat x;
    GLfloat y;
};

void Graph3DComponent::newOpenGLContextCreated ()
{
    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    static constexpr GLsizei kGraphSize = 256;
    std::array<std::array<GLbyte, kGraphSize>, kGraphSize> graph {};

    for (int i = 0; i < kGraphSize; i++)
    {
        for (int j = 0; j < kGraphSize; j++)
        {
            float x = (i - kGraphSize / 2.f) / (kGraphSize / 2.0f);
            float y = (j - kGraphSize / 2.f) / (kGraphSize / 2.0f);
            float d = std::hypotf (x, y) * 4.0f;
            float z = (1.0f - d * d) * std::expf (d * d / -2.0f);
            graph [i][j] = std::roundf (z * 127.f + 128.f);
        }
    }

    GLCall (open_gl_context_.extensions.glActiveTexture (juce::gl::GL_TEXTURE0));
    GLCall (juce::gl::glGenTextures (1, &uniform_texture_));
    GLCall (juce::gl::glBindTexture (juce::gl::GL_TEXTURE_2D, uniform_texture_));
    GLCall (juce::gl::glTexImage2D (juce::gl::GL_TEXTURE_2D,
                                    0,
                                    juce::gl::GL_RED,
                                    kGraphSize,
                                    kGraphSize,
                                    0,
                                    juce::gl::GL_RED,
                                    juce::gl::GL_BYTE,
                                    graph.data ()));

    static constexpr size_t kVertexBufferSize = 101;
    std::array<std::array<Vec2, kVertexBufferSize>, kVertexBufferSize> vertices {};
    for (int i = 0; i < kVertexBufferSize; i++)
    {
        for (int j = 0; j < kVertexBufferSize; j++)
        {
            vertices [i][j].x = (j - 50.f) / 50.0f;
            vertices [i][j].y = (i - 50.f) / 50.0f;
        }
    }
    vertex_buffer_ =
        std::make_unique<VertexBuffer> (open_gl_context_, vertices.data (), sizeof (vertices));

    std::array<GLuint, 100 * 101 * 4> indices;
    int i = 0;
    for (int y = 0; y < 101; y++)
    {
        for (int x = 0; x < 100; x++)
        {
            indices [i++] = y * 101 + x;
            indices [i++] = y * 101 + x + 1;
        }
    }

    for (int x = 0; x < 101; x++)
    {
        for (int y = 0; y < 100; y++)
        {
            indices [i++] = y * 101 + x;
            indices [i++] = (y + 1) * 101 + x;
        }
    }
    index_buffer_ =
        std::make_unique<IndexBuffer> (open_gl_context_, indices.data (), indices.size ());

    vertex_array_ = std::make_unique<VertexArray> (open_gl_context_);
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<GLfloat> (2);
    vertex_array_->AddBuffer (*vertex_buffer_, vertex_buffer_layout);
}

void Graph3DComponent::openGLContextClosing ()
{
    shader.reset ();
}

void Graph3DComponent::renderOpenGL ()
{
    jassert (juce::OpenGLHelpers::isContextActive ());
    juce::OpenGLHelpers::clear (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    CreateShaders ();
    shader->use ();

    juce::gl::glUniform1i (uniform_texture_, 0);

    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_S, juce::gl::GL_CLAMP_TO_EDGE);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_WRAP_T, juce::gl::GL_CLAMP_TO_EDGE);

    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MIN_FILTER, juce::gl::GL_LINEAR);
    juce::gl::glTexParameteri (
        juce::gl::GL_TEXTURE_2D, juce::gl::GL_TEXTURE_MAG_FILTER, juce::gl::GL_LINEAR);

    vertex_array_->Bind ();
    index_buffer_->Bind ();
    GLCall (juce::gl::glDrawElements (
                juce::gl::GL_LINES, 100 * 101 * 4, juce::gl::GL_UNSIGNED_INT, 0););
    index_buffer_->Unbind ();
    vertex_array_->Unbind ();
}

void Graph3DComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (refresh_button_).withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void Graph3DComponent::CreateShaders ()
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
            uniform_texture_ = open_gl_context_.extensions.glGetUniformLocation (
                shader->getProgramID (), "graph_texture");
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

void Graph3DComponent::UpdateShaders ()
{
    juce::SpinLock::ScopedLockType lock (shader_mutex_);

#if JUCE_DEBUG
    static const auto fragment_shader_filepath = kShaderDirectory / "graph3d.frag.glsl";
    static const auto vertex_shader_filepath = kShaderDirectory / "graph3d.vert.glsl";

    auto fragment_shader_file = juce::File (fragment_shader_filepath.string ());
    auto vertex_shader_file = juce::File (vertex_shader_filepath.string ());

    new_fragment_shader_ = fragment_shader_file.loadFileAsString ();
    new_vertex_shader_ = vertex_shader_file.loadFileAsString ();
#elif
    new_fragment_shader_ = shaders_graph3d_frag_glsl;
    new_vertex_shader_ = shaders_graph3d_vert_glsl;
#endif
}