zones_add_module(
        MODULE spectrogram_visualiser
        SOURCES src/spectrogram_visualiser/SpectrogramVisualiserComponent.h
        src/spectrogram_visualiser/SpectrogramVisualiserComponent.cpp
        src/spectrogram_visualiser/RingBuffer.h
        src/spectrogram_visualiser/GLUtils.h
        src/spectrogram_visualiser/GraphComponent.h
        src/spectrogram_visualiser/GraphComponent.cpp
        src/spectrogram_visualiser/Graph3DComponent.h
        src/spectrogram_visualiser/Graph3DComponent.cpp
        src/spectrogram_visualiser/VertexBuffer.h
        src/spectrogram_visualiser/VertexBuffer.cpp
        src/spectrogram_visualiser/IndexBuffer.h
        src/spectrogram_visualiser/IndexBuffer.cpp
        src/spectrogram_visualiser/VertexArray.h
        src/spectrogram_visualiser/VertexArray.cpp
        src/spectrogram_visualiser/VertexBufferLayout.h
        src/spectrogram_visualiser/Shader.h
        src/spectrogram_visualiser/Shader.cpp
        src/spectrogram_visualiser/DraggableOrientation.h
        src/spectrogram_visualiser/DraggableOrientation.cpp
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
