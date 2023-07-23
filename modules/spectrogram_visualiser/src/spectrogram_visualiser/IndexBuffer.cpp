#include "IndexBuffer.h"

#include "GLUtils.h"

IndexBuffer::IndexBuffer (juce::OpenGLContext & open_gl_context, const GLuint * data, GLuint count)
    : open_gl_context_ (open_gl_context)
{
    GLCall (open_gl_context_.extensions.glGenBuffers (1, &renderer_id_));
    Bind ();
    GLCall (open_gl_context_.extensions.glBufferData (juce::gl::GL_ELEMENT_ARRAY_BUFFER,
                                                      count * sizeof (GLuint),
                                                      data,
                                                      juce::gl::GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer ()
{
    GLCall (open_gl_context_.extensions.glDeleteBuffers (1, &renderer_id_));
}

void IndexBuffer::Bind () const
{
    GLCall (
        open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}

void IndexBuffer::Unbind () const
{
    GLCall (open_gl_context_.extensions.glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLuint IndexBuffer::GetCount () const
{
    return count_;
}
