zones_add_module(
        MODULE visualiser
        SOURCES
        src/visualiser/component/WaterfallComponent.h
        src/visualiser/component/WaterfallComponent.cpp
        src/visualiser/component/DraggableOrientation.h
        src/visualiser/component/DraggableOrientation.cpp
        src/visualiser/renderer/WaterfallRenderer.h
        src/visualiser/renderer/WaterfallRenderer.cpp
        src/visualiser/renderer/Spectrogram.h
        src/visualiser/renderer/Spectrogram.cpp
        src/visualiser/gl/GLUtils.h
        src/visualiser/gl/VertexBuffer.h
        src/visualiser/gl/VertexBuffer.cpp
        src/visualiser/gl/IndexBuffer.h
        src/visualiser/gl/IndexBuffer.cpp
        src/visualiser/gl/VertexArray.h
        src/visualiser/gl/VertexArray.cpp
        src/visualiser/gl/VertexBufferLayout.h
        src/visualiser/renderer/WaterfallGraph.h
        src/visualiser/renderer/WaterfallGraph.cpp
        src/visualiser/gl/DynamicShaderLoader.h
        src/visualiser/gl/DynamicShaderLoader.cpp
        DEPS
        juce::juce_gui_basics
        juce::juce_opengl
        look_and_feel
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_audio_formats
        glm
        tinycolormap
        immer
)

embed_resources(visualiser
        shaders/graph3d.frag.glsl
        shaders/graph3d.vert.glsl
        shaders/grid3d.frag.glsl)

target_compile_definitions(
        visualiser
        PUBLIC "SHADER_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/shaders/\"")
