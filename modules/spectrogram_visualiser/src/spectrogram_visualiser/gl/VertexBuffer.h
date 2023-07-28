#pragma once
#include <juce_opengl/juce_opengl.h>

class VertexBuffer
{
public:
    VertexBuffer (const void * data, GLuint size);
    ~VertexBuffer ();

    void Bind () const;
    void Unbind () const;

private:
    GLuint renderer_id_ {};
};