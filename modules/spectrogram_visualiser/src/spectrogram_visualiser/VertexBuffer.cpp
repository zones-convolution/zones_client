#include "VertexBuffer.h"

#include "GLUtils.h"

VertexBuffer::VertexBuffer (juce::OpenGLContext & open_gl_context, const void * data, GLuint size)
    : open_gl_context_ (open_gl_context)
{
    GLCall (open_gl_context_.extensions.glGenBuffers (1, &renderer_id_));
    Bind ();
    GLCall (open_gl_context_.extensions.glBufferData (
        juce::gl::GL_ARRAY_BUFFER, size, data, juce::gl::GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer ()
{
    GLCall (open_gl_context_.extensions.glDeleteBuffers (1, &renderer_id_));
}

void VertexBuffer::Bind () const
{
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::Unbind () const
{
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0));
}
