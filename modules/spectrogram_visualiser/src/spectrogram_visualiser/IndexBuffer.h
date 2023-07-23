#pragma once
#include <juce_opengl/juce_opengl.h>

class IndexBuffer
{
public:
    IndexBuffer (juce::OpenGLContext & open_gl_context, const GLuint * data, GLuint count);
    ~IndexBuffer ();

    void Bind () const;
    void Unbind () const;

    [[nodiscard]] inline GLuint GetCount () const;

private:
    juce::OpenGLContext & open_gl_context_;
    GLuint renderer_id_ {};
    GLuint count_ {};
};
