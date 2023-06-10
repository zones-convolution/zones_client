#include "SpectrogramVisualiserComponent.h"

SpectrogramVisualiserComponent::SpectrogramVisualiserComponent ()
{
    setOpaque (true);

    opengl_context_.setRenderer (this);
    opengl_context_.setContinuousRepainting (true);
    opengl_context_.attachTo (*this);
}

SpectrogramVisualiserComponent::~SpectrogramVisualiserComponent ()
{
    opengl_context_.detach ();
}

void SpectrogramVisualiserComponent::resized ()
{
}

void SpectrogramVisualiserComponent::paint (juce::Graphics & g)
{
}

void SpectrogramVisualiserComponent::newOpenGLContextCreated ()
{
    opengl_context_.extensions.glGenBuffers (1, &vertex_buffer_object_);
    opengl_context_.extensions.glGenBuffers (1, &index_buffer_object_);

    vertex_buffer_ = {{{-0.5f, 0.5f}, {1.f, 0.f, 0.f, 1.f}},
                      {{0.5f, 0.5f}, {1.f, 0.5f, 0.f, 1.f}},
                      {{0.5f, -0.5f}, {1.f, 1.f, 0.f, 1.f}},
                      {{-0.5f, -0.5f}, {1.f, 0.f, 1.f, 1.f}}};

    index_buffer_ = {0, 1, 2, 0, 2, 3};

    opengl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vertex_buffer_object_);

    // Send the vertices data.
    opengl_context_.extensions.glBufferData (juce::gl::GL_ARRAY_BUFFER,
                                             sizeof (Vertex) * vertex_buffer_.size (),
                                             vertex_buffer_.data (),
                                             juce::gl::GL_STATIC_DRAW);

    opengl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER,
                                             index_buffer_object_);

    opengl_context_.extensions.glBufferData (
        juce::gl::GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr> (sizeof (unsigned int) * index_buffer_.size ()),
        index_buffer_.data (),
        juce::gl::GL_STATIC_DRAW);

    shader_program_ = std::make_unique<juce::OpenGLShaderProgram> (opengl_context_);
    auto vertex_shader = juce::String (BinaryData::test_vert_glsl);
    auto fragment_shader = juce::String (BinaryData::test_frag_glsl);

    if (shader_program_->addVertexShader (vertex_shader) &&
        shader_program_->addFragmentShader (fragment_shader) && shader_program_->link ())
        shader_program_->use ();
    else
        jassertfalse;
}

void SpectrogramVisualiserComponent::renderOpenGL ()
{
    juce::OpenGLHelpers::clear (juce::Colours::black);
    shader_program_->use ();

    opengl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vertex_buffer_object_);
    opengl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER,
                                             index_buffer_object_);

    opengl_context_.extensions.glVertexAttribPointer (
        0, 2, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, sizeof (Vertex), nullptr);

    opengl_context_.extensions.glEnableVertexAttribArray (0);

    opengl_context_.extensions.glVertexAttribPointer (1,
                                                      4,
                                                      juce::gl::GL_FLOAT,
                                                      juce::gl::GL_FALSE,
                                                      sizeof (Vertex),
                                                      (GLvoid *) (sizeof (float) * 2));

    opengl_context_.extensions.glEnableVertexAttribArray (1);

    juce::gl::glDrawElements (
        juce::gl::GL_TRIANGLES, index_buffer_.size (), juce::gl::GL_UNSIGNED_INT, nullptr);

    opengl_context_.extensions.glDisableVertexAttribArray (0);
    opengl_context_.extensions.glDisableVertexAttribArray (1);
}

void SpectrogramVisualiserComponent::openGLContextClosing ()
{
}
