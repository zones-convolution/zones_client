#include "DynamicShaderLoader.h"

DynamicShaderLoader::DynamicShaderLoader (
    const std::filesystem::path & shader_directory,
    const std::optional<ShaderLoader> & vertex_shader_loader,
    const std::optional<ShaderLoader> & fragment_shader_loader)
    : shader_directory_ (shader_directory)
    , vertex_shader_loader_ (vertex_shader_loader)
    , fragment_shader_loader_ (fragment_shader_loader)
{
}

void DynamicShaderLoader::Load ()
{
    juce::SpinLock::ScopedLockType lock (mutex_);

#if JUCE_DEBUG
    if (fragment_shader_loader_.has_value ())
    {
        auto fragment_shader_path = shader_directory_ / fragment_shader_loader_->shader_file;
        auto fragment_shader_file = juce::File (fragment_shader_path.string ());
        auto fragment_shader = fragment_shader_file.loadFileAsString ();
        fragment_shader_ = fragment_shader.toStdString ();
    }
    else
    {
        fragment_shader_ = std::nullopt;
    }

    if (vertex_shader_loader_.has_value ())
    {
        auto vertex_shader_path = shader_directory_ / vertex_shader_loader_->shader_file;
        auto vertex_shader_file = juce::File (vertex_shader_path.string ());
        auto vertex_shader = vertex_shader_file.loadFileAsString ();
        vertex_shader_ = vertex_shader.toStdString ();
    }
    else
    {
        vertex_shader_ = std::nullopt;
    }
#else
    if (fragment_shader_loader_.has_value ())
        fragment_shader_ = fragment_shader_loader_->shader_value;
    else
        fragment_shader_ = std::nullopt;

    if (vertex_shader_loader_.has_value ())
        vertex_shader_ = vertex_shader_loader_->shader_value;
    else
        vertex_shader_ = std::nullopt;
#endif
}

void DynamicShaderLoader::Update (juce::OpenGLShaderProgram & shader_program)
{
    juce::SpinLock::ScopedTryLockType lock (mutex_);
    if (! lock.isLocked ())
        return;

    if (vertex_shader_.has_value () || fragment_shader_.has_value ())
    {
        shader_program.release ();

        if (vertex_shader_.has_value ())
            shader_program.addVertexShader (*vertex_shader_);
        if (fragment_shader_.has_value ())
            shader_program.addFragmentShader (*fragment_shader_);

        shader_program.link ();

        vertex_shader_ = std::nullopt;
        fragment_shader_ = std::nullopt;
    }
}