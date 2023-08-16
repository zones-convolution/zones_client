#include "Shader.h"

Shader::Shader ()
{
}

Shader::~Shader ()
{
}

bool Shader::CompileShader ()
{
    return false;
}

void Shader::Bind ()
{
}

void Shader::Unbind ()
{
}

void Shader::SetUniform4f (const std::string & name, float v0, float v1, float f2, float f3)
{
}

GLuint Shader::GetUniformLocation (const std::string & name)
{
    return 0;
}
