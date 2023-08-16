#include "VertexArray.h"

VertexArray::VertexArray ()
{
    GLCall (juce::gl::glGenVertexArrays (1, &renderer_));
}

VertexArray::~VertexArray ()
{
    GLCall (juce::gl::glDeleteVertexArrays (1, &renderer_));
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
        GLCall (juce::gl::glEnableVertexAttribArray (i));
        GLCall (juce::gl::glVertexAttribPointer (i,
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
    GLCall (juce::gl::glBindVertexArray (renderer_));
}

void VertexArray::Unbind () const
{
    GLCall (juce::gl::glBindVertexArray (0));
}
