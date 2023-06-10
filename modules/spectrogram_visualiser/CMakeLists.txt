juce_add_binary_data(spectrogram_visualiser_shaders_binary_resource SOURCES shaders/test.frag.glsl shaders/test.vert.glsl)

zones_add_module(
        MODULE spectrogram_visualiser
        SOURCES src/spectrogram_visualiser/SpectrogramVisualiserComponent.h src/spectrogram_visualiser/SpectrogramVisualiserComponent.cpp
        DEPS juce::juce_gui_basics juce::juce_opengl spectrogram_visualiser_shaders_binary_resource
)