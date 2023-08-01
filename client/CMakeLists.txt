set(BINARY zones_client)

set(LINK_JUCE juce::juce_gui_extra juce::juce_audio_devices
        juce::juce_audio_utils)

set(LINK_COMMON
        ir_repository
        ir_engine
        zones_look_and_feel
        audio_engine
        model
        layout)

# ##############################################################################
# SRC
# ##############################################################################
set(SOURCES src/PluginProcessor.h src/PluginProcessor.cpp src/PluginEditor.h
        src/PluginEditor.cpp src/ProjectImportComponent.h src/ProjectImportComponent.cpp)

juce_add_plugin(
        ${BINARY}
        PLUGIN_MANUFACTURER_CODE
        Zone
        PLUGIN_CODE
        Zone
        FORMATS
        AU
        VST3
        Standalone
        COPY_PLUGIN_AFTER_BUILD
        TRUE
        PRODUCT_NAME
        Zones)

target_sources(${BINARY} PRIVATE ${SOURCES})

target_link_libraries(
        ${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON}
        PUBLIC ${MODULE_LINK_CONF})

target_compile_options(${BINARY} INTERFACE -Wall -Wextra -Werror)

# ##############################################################################
# TEST
# ##############################################################################
# set(BINARY_TST ${BINARY}_test) add_executable(${BINARY_TST}
# ../modules/browser/src/browser/Browser.cpp
# ../modules/browser/src/browser/Browser.h)
# target_include_directories(${BINARY_TST} PRIVATE src)
# target_sources(${BINARY_TST} PRIVATE ${SOURCES}) target_link_libraries(
# ${BINARY_TST} PRIVATE ${LINK_JUCE} Catch2::Catch2WithMain PUBLIC
# ${MODULE_LINK_CONF})
