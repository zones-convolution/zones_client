#include "VertexArray.h"

VertexArray::VertexArray (juce::OpenGLContext & open_gl_context)
    : open_gl_context_ (open_gl_context)
{
    GLCall (open_gl_context_.extensions.glGenVertexArrays (1, &renderer_));
}

VertexArray::~VertexArray ()
{
    GLCall (open_gl_context_.extensions.glDeleteVertexArrays (1, &renderer_));
}

void VertexArray::AddBuffer (const VertexBuffer & vertex_buffer,
                             const VertexBufferLayout & vertex_buffer_layout) const
{
    Bind ();
    vertex_buffer.Bind ();
    const auto & elements = vertex_buffer_layout.GetElements ();
    GLuint offset = 0;
    for (GLuint i = 0; i < elements.size (); ++i)
    {
        const auto & element = elements [i];
        GLCall (open_gl_context_.extensions.glEnableVertexAttribArray (i));
        GLCall (
            open_gl_context_.extensions.glVertexAttribPointer (i,
                                                               element.count,
                                                               element.type,
                                                               element.normalised,
                                                               vertex_buffer_layout.GetStride (),
                                                               (const void *) offset));

        offset +=
            static_cast<GLuint> (element.count) * VertexBufferElement::GetSizeOfType (element.type);
    }
}

void VertexArray::Bind () const
{
    GLCall (open_gl_context_.extensions.glBindVertexArray (renderer_));
}

void VertexArray::Unbind () const
{
    GLCall (open_gl_context_.extensions.glBindVertexArray (0));
}
