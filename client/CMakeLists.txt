set(BINARY zones_client)

set(LINK_JUCE
        juce::juce_gui_extra
        juce::juce_audio_devices
        juce::juce_audio_utils)

set(LINK_COMMON
        format
        ir_engine
        audio_engine
        model
        zones_convolver
        preferences
        rocket)

set(SOURCES
        src/PluginProcessor.h
        src/PluginProcessor.cpp
        src/PluginEditor.h
        src/PluginEditor.cpp
        src/ProcessorContainer.h
        src/ProcessorContainer.cpp
        src/WithJuceEventLoop.h
        src/relays/EngineRelay.h
        src/relays/EngineRelay.cpp
        src/relays/PlayerRelay.h
        src/relays/PlayerRelay.cpp
        src/relays/PreferencesRelay.h
        src/relays/PreferencesRelay.cpp
        src/relays/MeteringRelay.h
        src/relays/MeteringRelay.cpp
        src/relays/ResizeRelay.h
        src/relays/ResizeRelay.cpp
        src/relays/ParameterRelay.h
        src/relays/ParameterRelay.cpp
        src/relays/VisualiserRelay.h
        src/relays/VisualiserRelay.cpp
        src/relays/LoadRelay.h
        src/relays/LoadRelay.cpp
        src/relays/UserZonesRelay.h
        src/relays/UserZonesRelay.cpp
        src/relays/WebZonesRelay.h
        src/relays/WebZonesRelay.cpp
        src/controllers/visualiser/VisualiserController.h
        src/controllers/visualiser/VisualiserController.cpp
        src/controllers/visualiser/Spectrogram.h
        src/controllers/visualiser/Spectrogram.cpp
        src/controllers/LoadController.h
        src/controllers/LoadController.cpp
        src/controllers/UserZonesController.h
        src/controllers/UserZonesController.cpp
        src/controllers/WebZonesController.h
        src/controllers/WebZonesController.cpp
        src/controllers/WebLoadController.h
        src/controllers/WebLoadController.cpp
)

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/dist/)

juce_add_plugin(
        ${BINARY}
        PLUGIN_MANUFACTURER_CODE Zone
        PLUGIN_CODE Zone
        PRODUCT_NAME "${PRODUCT_NAME}"
        COMPANY_NAME "${COMPANY_NAME}"
        BUNDLE_ID "${BUNDLE_ID}"
        VERSION "${CURRENT_VERSION}"
        FORMATS Standalone VST3 AU
        COPY_PLUGIN_AFTER_BUILD TRUE
        NEEDS_WEBVIEW2 TRUE
        NEEDS_WEB_BROWSER TRUE
        NEEDS_CURL TRUE)

target_compile_definitions(${BINARY}
        PRIVATE
        "DEV_LOCALHOST=$<BOOL:$ENV{DEV_LOCALHOST}>"
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=1
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_sources(${BINARY} PRIVATE ${SOURCES})

target_link_libraries(
        ${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON}
        PUBLIC ${MODULE_LINK_CONF})

target_compile_options(${BINARY} INTERFACE -Wall -Wextra -Werror)

if (NOT DEV_LOCALHOST AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    get_target_property(ACTIVE_TARGETS ${BINARY} JUCE_ACTIVE_PLUGIN_TARGETS)
    foreach (ACTIVE_TARGET IN LISTS ACTIVE_TARGETS)
        message(STATUS "Adding resource copy step from ${ASSETS_DIR} for ${ACTIVE_TARGET}")

        get_target_property(ARTIFACT_FILE ${ACTIVE_TARGET} JUCE_PLUGIN_ARTEFACT_FILE)
        set(RESOURCE_DIR "${ARTIFACT_FILE}/Contents/Resources")

        add_custom_command(TARGET ${ACTIVE_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rm -rf "${RESOURCE_DIR}/dist"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DIR}/dist"
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${RESOURCE_DIR}/dist"
                VERBATIM)
    endforeach ()
endif ()