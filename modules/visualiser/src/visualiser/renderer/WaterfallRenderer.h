#pragma once

#include "../component/DraggableOrientation.h"
#include "../gl/DynamicShaderLoader.h"
#include "Spectrogram.h"
#include "WaterfallGraph.h"

#include <filesystem>
#include <juce_opengl/juce_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class CreateTextureJob : public juce::ThreadPoolJob
{
public:
    CreateTextureJob (const immer::box<juce::AudioBuffer<float>> & boxed_buffer,
                      std::function<void (const juce::Image & texture)> callback);
    ~CreateTextureJob () override = default;
    JobStatus runJob () override;

private:
    immer::box<juce::AudioBuffer<float>> boxed_buffer_;
    std::function<void (const juce::Image & texture)> callback_;
};

class WaterfallRenderer final : public juce::OpenGLRenderer
{
public:
    WaterfallRenderer (juce::OpenGLContext & open_gl_context,
                       DraggableOrientation & draggable_orientation,
                       juce::ThreadPool & thread_pool);

    ~WaterfallRenderer () override = default;
    void newOpenGLContextCreated () override;
    void renderOpenGL () override;
    void openGLContextClosing () override;

    void UpdateShaders ();
    void LoadParameters (const WaterfallGraph::Parameters & parameters);

    std::atomic<float> offset_x_;
    std::atomic<float> offset_y_;
    std::atomic<float> scale_;

    void SetupGraphTexture (const immer::box<juce::AudioBuffer<float>> & boxed_buffer);

private:
    static const std::filesystem::path kShaderDirectory;

    glm::mat4 CreateTextureTransform () const;
    glm::mat4 CreateVertexTransform ();

    DraggableOrientation & draggable_orientation_;
    float rot_x_smooth_ = 0.f;
    float rot_y_smooth_ = 0.f;

    juce::OpenGLContext & open_gl_context_;
    DynamicShaderLoader graph_shader_loader_;
    DynamicShaderLoader grid_shader_loader_;

    std::mutex graph_mutex_;
    WaterfallGraph waterfall_graph_ {open_gl_context_, graph_shader_loader_, grid_shader_loader_};

    juce::ThreadPool & thread_pool_;
    juce::ThreadPoolJob * last_texture_job_ = nullptr;
};
