#pragma once
#include <juce_opengl/juce_opengl.h>

class IndexBuffer
{
public:
    IndexBuffer (const GLuint * data, GLuint count);
    ~IndexBuffer ();

    void Bind () const;
    void Unbind () const;

    [[nodiscard]] inline GLuint GetCount () const;

private:
    GLuint renderer_id_ {};
    GLuint count_ {};
};
