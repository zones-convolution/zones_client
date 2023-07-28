#include "IndexBuffer.h"

#include "GLUtils.h"

IndexBuffer::IndexBuffer (const GLuint * data, GLuint count)
{
    GLCall (juce::gl::glGenBuffers (1, &renderer_id_));
    Bind ();
    GLCall (juce::gl::glBufferData (juce::gl::GL_ELEMENT_ARRAY_BUFFER,
                                    count * sizeof (GLuint),
                                    data,
                                    juce::gl::GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer ()
{
    GLCall (juce::gl::glDeleteBuffers (1, &renderer_id_));
}

void IndexBuffer::Bind () const
{
    GLCall (juce::gl::glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}

void IndexBuffer::Unbind () const
{
    GLCall (juce::gl::glBindBuffer (juce::gl::GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLuint IndexBuffer::GetCount () const
{
    return count_;
}
