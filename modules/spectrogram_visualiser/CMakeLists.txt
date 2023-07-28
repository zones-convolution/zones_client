zones_add_module(
        MODULE spectrogram_visualiser
        SOURCES src/spectrogram_visualiser/component/Osc2DComponent.h
        src/spectrogram_visualiser/component/Osc2DComponent.cpp
        src/spectrogram_visualiser/component/GraphComponent.h
        src/spectrogram_visualiser/component/GraphComponent.cpp
        src/spectrogram_visualiser/component/Graph3DComponent.h
        src/spectrogram_visualiser/component/Graph3DComponent.cpp
        src/spectrogram_visualiser/component/DraggableOrientation.h
        src/spectrogram_visualiser/component/DraggableOrientation.cpp
        src/spectrogram_visualiser/renderer/Graph3DRenderer.h
        src/spectrogram_visualiser/renderer/Graph3DRenderer.cpp
        src/spectrogram_visualiser/gl/GLUtils.h
        src/spectrogram_visualiser/gl/VertexBuffer.h
        src/spectrogram_visualiser/gl/VertexBuffer.cpp
        src/spectrogram_visualiser/gl/IndexBuffer.h
        src/spectrogram_visualiser/gl/IndexBuffer.cpp
        src/spectrogram_visualiser/gl/VertexArray.h
        src/spectrogram_visualiser/gl/VertexArray.cpp
        src/spectrogram_visualiser/gl/VertexBufferLayout.h
        DEPS juce::juce_gui_basics juce::juce_opengl zones_look_and_feel juce::juce_audio_basics juce::juce_dsp juce::juce_audio_formats glm tinycolormap
)

embed_resources(spectrogram_visualiser
        shaders/osc_2d.frag.glsl
        shaders/osc_2d.vert.glsl
        shaders/graph.frag.glsl
        shaders/graph.vert.glsl
        shaders/graph3d.frag.glsl
        shaders/graph3d.vert.glsl)

target_compile_definitions(
        spectrogram_visualiser
        PUBLIC "SHADER_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/shaders/\"" "TEST_AUDIO_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_audio/\"")
