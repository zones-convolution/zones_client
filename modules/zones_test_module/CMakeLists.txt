zones_add_module(
    MODULE zones_test_module
    SOURCES src/zones_test_module/TestModuleClass.cpp
        src/zones_test_module/TestModuleClass.h
    DEPS juce::juce_gui_extra
        juce::juce_audio_devices
        juce::juce_audio_utils
)

zones_add_test(
    MODULE zones_test_module
    SOURCES test/TestTestModuleClass.cpp
    DEPS zones_test_module
)