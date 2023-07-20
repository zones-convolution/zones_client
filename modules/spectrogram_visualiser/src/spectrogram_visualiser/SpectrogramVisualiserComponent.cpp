#include "SpectrogramVisualiserComponent.h"

#include <memory>

extern "C" const char shaders_osc_3d_frag_glsl [];
extern "C" const unsigned shaders_osc_3d_frag_glsl_size;

extern "C" const char shaders_osc_3d_vert_glsl [];
extern "C" const unsigned shaders_osc_3d_vert_glsl_size;

SpectrogramVisualiserComponent::SpectrogramVisualiserComponent (RingBuffer<GLfloat> * ringBuffer)
    : ring_buffer_ (ringBuffer)
    , read_buffer_ (2, kRingBufferReadSize)
{
    setOpaque (true);
    open_gl_context_.setOpenGLVersionRequired (juce::OpenGLContext::OpenGLVersion::openGL4_1);

    if (auto * peer = getPeer ())
        peer->setCurrentRenderingEngine (0);

    open_gl_context_.setRenderer (this);
    open_gl_context_.attachTo (*this);

    addAndMakeVisible (status_label_);
    status_label_.setJustificationType (juce::Justification::topLeft);
}

void SpectrogramVisualiserComponent::paint (juce::Graphics & g)
{
}

SpectrogramVisualiserComponent::~SpectrogramVisualiserComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

void SpectrogramVisualiserComponent::Start ()
{
    open_gl_context_.setContinuousRepainting (true);
}

void SpectrogramVisualiserComponent::Stop ()
{
    open_gl_context_.setContinuousRepainting (false);
}

void SpectrogramVisualiserComponent::newOpenGLContextCreated ()
{
    CreateShaders ();
    open_gl_context_.extensions.glGenBuffers (1, &vbo_);
    open_gl_context_.extensions.glGenBuffers (1, &ebo_);
}

void SpectrogramVisualiserComponent::openGLContextClosing ()
{
    shader.reset ();
    uniforms.reset ();
}

void SpectrogramVisualiserComponent::renderOpenGL ()
{
    jassert (juce::OpenGLHelpers::isContextActive ());

    const auto kRenderingScale = (float) open_gl_context_.getRenderingScale ();
    juce::gl::glViewport (0,
                          0,
                          juce::roundToInt (kRenderingScale * (float) getWidth ()),
                          juce::roundToInt (kRenderingScale * (float) getHeight ()));

    juce::OpenGLHelpers::clear (getLookAndFeel ().findColour (LookAndFeel::ColourIds::kPanel));

    juce::gl::glEnable (juce::gl::GL_BLEND);
    juce::gl::glBlendFunc (juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA);

    shader->use ();

    if (uniforms->resolution != nullptr)
        uniforms->resolution->set ((GLfloat) kRenderingScale * getWidth (),
                                   (GLfloat) kRenderingScale * getHeight ());

    // Read in samples from ring buffer
    if (uniforms->audio_sample_data != nullptr)
    {
        juce::AudioBuffer<float> buff;
        buff.setSize (2, kRingBufferReadSize);
        juce::Random rand;
        for (auto channel = 0; channel < buff.getNumChannels (); ++channel)
        {
            auto * buffer = buff.getWritePointer (channel, 0);
            for (auto sample = 0; sample < buff.getNumSamples (); ++sample)
                buffer [sample] = rand.nextFloat () * 0.25f - 0.125f;
        }

        ring_buffer_->writeSamples (buff, 0, kRingBufferReadSize);
        ring_buffer_->readSamples (read_buffer_, kRingBufferReadSize);
        juce::FloatVectorOperations::clear (visualization_buffer_, kRingBufferReadSize);

        // Sum channels together
        for (int i = 0; i < 2; ++i)
            juce::FloatVectorOperations::add (
                visualization_buffer_, read_buffer_.getReadPointer (i, 0), kRingBufferReadSize);

        uniforms->audio_sample_data->set (visualization_buffer_, 256);
    }

    // Define Vertices for a Square (the view plane)
    GLfloat vertices [] = {
        1.0f,
        1.0f,
        0.0f, // Top Right
        1.0f,
        -1.0f,
        0.0f, // Bottom Right
        -1.0f,
        -1.0f,
        0.0f, // Bottom Left
        -1.0f,
        1.0f,
        0.0f // Top Left
    };

    // Define Which Vertex Indexes Make the Square
    GLuint indices [] = {
        // Note that we start from 0!
        0,
        1,
        3, // First Triangle
        1,
        2,
        3 // Second Triangle
    };

    // This appears to be required! see this -
    // https://stackoverflow.com/questions/48714591/modern-opengl-macos-only-black-screen
    open_gl_context_.extensions.glGenVertexArrays (1, &vao_);
    open_gl_context_.extensions.glBindVertexArray (vao_);

    // VBO (Vertex Buffer Object) - Bind and Write to Buffer
    open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vbo_);
    open_gl_context_.extensions.glBufferData (
        juce::gl::GL_ARRAY_BUFFER, sizeof (vertices), vertices, juce::gl::GL_STREAM_DRAW);

    // EBO (Element Buffer Object) - Bind and Write to Buffer
    open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, ebo_);
    open_gl_context_.extensions.glBufferData (
        juce::gl::GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, juce::gl::GL_STREAM_DRAW);

    // Setup Vertex Attributes
    open_gl_context_.extensions.glVertexAttribPointer (
        0, 3, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, 3 * sizeof (GLfloat), nullptr);
    open_gl_context_.extensions.glEnableVertexAttribArray (0);

    // Draw Vertices
    // glDrawArrays (GL_TRIANGLES, 0, 6); // For just VBO's (Vertex Buffer Objects)
    juce::gl::glDrawElements (juce::gl::GL_TRIANGLES, 6, juce::gl::GL_UNSIGNED_INT, nullptr);

    // Reset the element buffers so child Components draw correctly
    open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0);
    open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpectrogramVisualiserComponent::resized ()
{
    status_label_.setBounds (getLocalBounds ().reduced (4).removeFromTop (75));
}

void SpectrogramVisualiserComponent::CreateShaders ()
{
    auto gl_shader_program = std::make_unique<juce::OpenGLShaderProgram> (open_gl_context_);

    juce::String status_text;
    if (gl_shader_program->addVertexShader (juce::StringRef (shaders_osc_3d_vert_glsl)) &&
        gl_shader_program->addFragmentShader (juce::StringRef (shaders_osc_3d_frag_glsl)) &&
        gl_shader_program->link ())
    {
        uniforms.reset ();
        shader = std::move (gl_shader_program);
        uniforms = std::make_unique<Uniforms> (open_gl_context_, *shader);

        status_text =
            "GLSL: v" + juce::String (juce::OpenGLShaderProgram::getLanguageVersion (), 2);
    }
    else
    {
        status_text = gl_shader_program->getLastError ();
    }

    juce::MessageManager::callAsync (
        [&, status_text] () { status_label_.setText (status_text, juce::dontSendNotification); });
}

juce::OpenGLShaderProgram::Uniform *
SpectrogramVisualiserComponent::Uniforms::CreateUniform (juce::OpenGLContext & open_gl_context,
                                                         juce::OpenGLShaderProgram & shader_program,
                                                         const char * uniform_name)
{
    if (open_gl_context.extensions.glGetUniformLocation (shader_program.getProgramID (),
                                                         uniform_name) < 0)
        return nullptr;

    return new juce::OpenGLShaderProgram::Uniform (shader_program, uniform_name);
}

SpectrogramVisualiserComponent::Uniforms::Uniforms (juce::OpenGLContext & open_gl_context,
                                                    juce::OpenGLShaderProgram & shader_program)
{
    resolution.reset (CreateUniform (open_gl_context, shader_program, "resolution"));
    audio_sample_data.reset (CreateUniform (open_gl_context, shader_program, "audioSampleData"));
}
