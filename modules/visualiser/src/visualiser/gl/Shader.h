#pragma once
#include <juce_opengl/juce_opengl.h>
class Shader
{
public:
    Shader ();
    ~Shader ();

    void Bind ();
    void Unbind ();

    void SetUniform4f (const std::string & name, float v0, float v1, float f2, float f3);

private:
    GLuint renderer_id_ {};

    bool CompileShader ();
    GLuint GetUniformLocation (const std::string & name);
};
