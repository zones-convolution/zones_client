add_subdirectory(JUCE)

add_library(rocket INTERFACE)
target_include_directories(rocket SYSTEM INTERFACE rocket)

add_library(immer INTERFACE)
target_include_directories(immer SYSTEM INTERFACE immer)

add_library(choc INTERFACE)
target_include_directories(choc SYSTEM INTERFACE choc)

juce_add_module(melatonin_test_helpers)
juce_add_module(melatonin_audio_sparklines)
juce_add_module(zones_convolver/zones_convolver)