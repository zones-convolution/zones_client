#include "Osc2DComponent.h"

#include "../gl/GLUtils.h"

#include <memory>

#if JUCE_DEBUG
const std::filesystem::path Osc2DComponent::kShaderDirectory = SHADER_DIRECTORY;
#elif
extern "C" const char shaders_osc_2d_frag_glsl [];
extern "C" const unsigned shaders_osc_2d_frag_glsl_size;

extern "C" const char shaders_osc_2d_vert_glsl [];
extern "C" const unsigned shaders_osc_2d_vert_glsl_size;
#endif

Osc2DComponent::Osc2DComponent ()
    : read_buffer_ (2, kRingBufferReadSize)
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

    juce::dsp::AudioBlock<float> read_block {read_buffer_};

    for (auto channel_index = 0; channel_index < read_block.getNumChannels (); ++channel_index)
    {
        auto * channel = read_block.getChannelPointer (channel_index);
        for (auto sample_index = 0; sample_index < read_block.getNumSamples (); ++sample_index)
        {
            channel [sample_index] = std::sinf (sample_index * 0.08f) / 4.f;
        }
    }
}

void Osc2DComponent::paint (juce::Graphics & g)
{
}

Osc2DComponent::~Osc2DComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

void Osc2DComponent::Start ()
{
    open_gl_context_.setContinuousRepainting (true);
}

void Osc2DComponent::Stop ()
{
    open_gl_context_.setContinuousRepainting (false);
}

void Osc2DComponent::newOpenGLContextCreated ()
{
    GLCall (juce::gl::glEnable (juce::gl::GL_BLEND));
    GLCall (juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA));

    std::array<GLfloat, 8> vertices {
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        1.0f,
    };
    vb_ = std::make_unique<VertexBuffer> (vertices.data (), sizeof (vertices));

    std::array<GLuint, 6> indices {0, 1, 2, 2, 3, 0};
    ib_ = std::make_unique<IndexBuffer> (indices.data (), indices.size ());

    va_ = std::make_unique<VertexArray> ();
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<GLfloat> (2);
    va_->AddBuffer (*vb_, vertex_buffer_layout);
}

void Osc2DComponent::openGLContextClosing ()
{
}

void Osc2DComponent::renderOpenGL ()
{
    CreateShaders ();
    jassert (juce::OpenGLHelpers::isContextActive ());

    const auto kRenderingScale = (float) open_gl_context_.getRenderingScale ();
    GLCall (juce::gl::glViewport (0,
                                  0,
                                  juce::roundToInt (kRenderingScale * (float) getWidth ()),
                                  juce::roundToInt (kRenderingScale * (float) getHeight ())));

    juce::OpenGLHelpers::clear (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    shader->use ();

    if (resolution_ != nullptr)
        resolution_->set ((GLfloat) kRenderingScale * getWidth (),
                          (GLfloat) kRenderingScale * getHeight ());

    if (audio_sample_data_ != nullptr)
    {
        juce::FloatVectorOperations::clear (visualization_buffer_, kRingBufferReadSize);
        for (auto channel_index = 0; channel_index < read_buffer_.getNumChannels ();
             ++channel_index)
            juce::FloatVectorOperations::add (visualization_buffer_,
                                              read_buffer_.getReadPointer (channel_index, 0),
                                              kRingBufferReadSize);
        audio_sample_data_->set (visualization_buffer_, 256);
    }

    va_->Bind ();
    ib_->Bind ();
    GLCall (
        juce::gl::glDrawElements (juce::gl::GL_TRIANGLES, 6, juce::gl::GL_UNSIGNED_INT, nullptr));
    ib_->Unbind ();
    va_->Unbind ();
}

void Osc2DComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (refresh_button_).withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void Osc2DComponent::CreateShaders ()
{
    juce::SpinLock::ScopedTryLockType lock (shader_mutex_);
    if (! lock.isLocked ())
        return;

    auto gl_shader_program = std::make_unique<juce::OpenGLShaderProgram> (open_gl_context_);

    static const juce::String kGLSLVersionLabel = "GLSL: v";
    if (new_vertex_shader_.isNotEmpty () || new_fragment_shader_.isNotEmpty ())
    {
        juce::String status_text;
        if (gl_shader_program->addVertexShader (new_vertex_shader_) &&
            gl_shader_program->addFragmentShader (new_fragment_shader_) &&
            gl_shader_program->link ())
        {
            shader = std::move (gl_shader_program);
            resolution_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader, "resolution");
            audio_sample_data_ =
                std::make_unique<juce::OpenGLShaderProgram::Uniform> (*shader, "audioSampleData");

            status_text = kGLSLVersionLabel +
                          juce::String (juce::OpenGLShaderProgram::getLanguageVersion (), 2);
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

void Osc2DComponent::UpdateShaders ()
{
    juce::SpinLock::ScopedLockType lock (shader_mutex_);

#if JUCE_DEBUG
    static const auto fragment_shader_filepath = kShaderDirectory / "osc_2d.frag.glsl";
    static const auto vertex_shader_filepath = kShaderDirectory / "osc_2d.vert.glsl";

    auto fragment_shader_file = juce::File (fragment_shader_filepath.string ());
    auto vertex_shader_file = juce::File (vertex_shader_filepath.string ());

    new_fragment_shader_ = fragment_shader_file.loadFileAsString ();
    new_vertex_shader_ = vertex_shader_file.loadFileAsString ();
#elif
    new_fragment_shader_ = shaders_osc_2d_frag_glsl;
    new_vertex_shader_ = shaders_osc_2d_vert_glsl;
#endif
}
