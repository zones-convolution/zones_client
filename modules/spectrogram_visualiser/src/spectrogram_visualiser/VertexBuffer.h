#pragma once
#include <juce_opengl/juce_opengl.h>

class VertexBuffer
{
public:
    VertexBuffer (juce::OpenGLContext & open_gl_context, const void * data, GLuint size);
    ~VertexBuffer ();

    void Bind () const;
    void Unbind () const;

private:
    juce::OpenGLContext & open_gl_context_;
    GLuint renderer_id_ {};
};