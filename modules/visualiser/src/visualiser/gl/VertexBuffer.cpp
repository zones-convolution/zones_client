#include "VertexBuffer.h"

#include "GLUtils.h"

VertexBuffer::VertexBuffer (const void * data, GLuint size)
{
    GLCall (juce::gl::glGenBuffers (1, &renderer_id_));
    Bind ();
    GLCall (
        juce::gl::glBufferData (juce::gl::GL_ARRAY_BUFFER, size, data, juce::gl::GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer ()
{
    GLCall (juce::gl::glDeleteBuffers (1, &renderer_id_));
}

void VertexBuffer::Bind () const
{
    GLCall (juce::gl::glBindBuffer (juce::gl::GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::Unbind () const
{
    GLCall (juce::gl::glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0));
}
