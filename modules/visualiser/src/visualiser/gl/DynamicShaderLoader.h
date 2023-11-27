#pragma once

#include <filesystem>
#include <juce_core/juce_core.h>
#include <juce_opengl/juce_opengl.h>

class DynamicShaderLoader
{
public:
    struct ShaderLoader
    {
        std::filesystem::path shader_file;
        std::string shader_value;
    };

    DynamicShaderLoader (const std::filesystem::path & shader_directory,
                         const std::optional<ShaderLoader> & vertex_shader_loader,
                         const std::optional<ShaderLoader> & fragment_shader_loader);

    void Load ();
    void Update (juce::OpenGLShaderProgram & shader_program);

private:
    juce::SpinLock mutex_;

    const std::filesystem::path shader_directory_;
    std::optional<ShaderLoader> vertex_shader_loader_;
    std::optional<ShaderLoader> fragment_shader_loader_;

    std::optional<std::string> vertex_shader_;
    std::optional<std::string> fragment_shader_;
};
