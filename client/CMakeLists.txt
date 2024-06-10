set(BINARY zones_client)

set(LINK_JUCE
        juce::juce_gui_extra
        juce::juce_audio_devices
        juce::juce_audio_utils)

set(LINK_COMMON
        zone_repository
        format
        ir_engine
        look_and_feel
        audio_engine
        model
        browser
        zones_convolver
        preferences)

set(SOURCES
        src/PluginProcessor.h
        src/PluginProcessor.cpp
        src/PluginEditor.h
        src/PluginEditor.cpp
        src/ProcessorContainer.h
        src/ProcessorContainer.cpp
        src/WithJuceEventLoop.h)

option(DEV_LOCALHOST OFF)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/dist/)

juce_add_plugin(
        ${BINARY}
        PLUGIN_MANUFACTURER_CODE Zone
        PLUGIN_CODE Zone
        PRODUCT_NAME Zones
        COMPANY_NAME Zones
        BUNDLE_ID com.zones.zones
        VERSION 0.0.2
        FORMATS AU VST3 Standalone
        COPY_PLUGIN_AFTER_BUILD TRUE
        NEEDS_WEBVIEW2 TRUE)

target_compile_definitions(${BINARY}
        PRIVATE
        "DEV_LOCALHOST=$<BOOL:${DEV_LOCALHOST}>"
        JUCE_WEB_BROWSER=1
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1)

target_sources(${BINARY} PRIVATE ${SOURCES})

target_link_libraries(
        ${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON}
        PUBLIC ${MODULE_LINK_CONF})

target_compile_options(${BINARY} INTERFACE -Wall -Wextra -Werror)

if (NOT DEV_LOCALHOST)
    get_target_property(ACTIVE_TARGETS ${BINARY} JUCE_ACTIVE_PLUGIN_TARGETS)
    foreach (ACTIVE_TARGET IN LISTS ACTIVE_TARGETS)
        message(STATUS "Adding resource copy step from ${ASSETS_DIR} for ${ACTIVE_TARGET}")

        get_target_property(ARTIFACT_FILE ${ACTIVE_TARGET} JUCE_PLUGIN_ARTEFACT_FILE)
        set(RESOURCE_DIR "${ARTIFACT_FILE}/Contents/Resources/")

        add_custom_command(TARGET ${ACTIVE_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rm -rf "${RESOURCE_DIR}/dist"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DIR}/dist"
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${RESOURCE_DIR}/dist"
                VERBATIM)
    endforeach ()
endif ()