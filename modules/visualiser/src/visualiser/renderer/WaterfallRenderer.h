#pragma once

#include "../component/DraggableOrientation.h"
#include "../gl/IndexBuffer.h"
#include "../gl/VertexArray.h"
#include "../gl/VertexBuffer.h"
#include "Spectrogram.h"

#include <filesystem>
#include <juce_opengl/juce_opengl.h>
#define GLM_FORCE_RADIANS
#include "WaterfallGraph.h"

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class WaterfallRenderer final : public juce::OpenGLRenderer
{
public:
    WaterfallRenderer (juce::OpenGLContext & open_gl_context,
                       DraggableOrientation & draggable_orientation);

    ~WaterfallRenderer () override = default;
    void newOpenGLContextCreated () override;
    void renderOpenGL () override;
    void openGLContextClosing () override;

    void UpdateShaders ();

    std::atomic<float> offset_x_;
    std::atomic<float> offset_y_;
    std::atomic<float> scale_;

    void SetupGraphTexture (juce::dsp::AudioBlock<const float> block);

private:
    static const std::filesystem::path kShaderDirectory;

    glm::mat4 CreateTextureTransform () const;
    glm::mat4 CreateVertexTransform ();

    DraggableOrientation & draggable_orientation_;
    float rot_x_smooth_ = 0.f;
    float rot_y_smooth_ = 0.f;

    juce::OpenGLContext & open_gl_context_;
    WaterfallGraph waterfall_graph_;
};
