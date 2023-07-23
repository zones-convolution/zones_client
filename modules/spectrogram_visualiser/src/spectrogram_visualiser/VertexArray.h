#pragma once

#include "GLUtils.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <juce_opengl/juce_opengl.h>

class VertexArray
{
public:
    explicit VertexArray (juce::OpenGLContext & open_gl_context);
    ~VertexArray ();

    void AddBuffer (const VertexBuffer & vertex_buffer,
                    const VertexBufferLayout & vertex_buffer_layout) const;

    void Bind () const;
    void Unbind () const;

private:
    juce::OpenGLContext & open_gl_context_;
    GLuint renderer_;
};
