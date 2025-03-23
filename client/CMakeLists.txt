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

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui/dist)

if (DEFINED ENV{CI} AND WIN32)
    execute_process(
            COMMAND nuget install Microsoft.Web.WebView2 -OutputDirectory "${CMAKE_BINARY_DIR}/_deps/Nuget"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )

    set(JUCE_WEBVIEW2_PACKAGE_LOCATION "${CMAKE_BINARY_DIR}/_deps/Nuget")
endif ()

set(FORMATS AU VST3)
set(SHOULD_COPY FALSE)
if (NOT DEFINED ENV{CI} AND APPLE)
    list(APPEND FORMATS Standalone)
    set(SHOULD_COPY TRUE)
endif ()

juce_add_plugin(
        ${BINARY}
        PLUGIN_MANUFACTURER_CODE Zone
        PLUGIN_CODE Zone
        PRODUCT_NAME "${PRODUCT_NAME}"
        COMPANY_NAME "${COMPANY_NAME}"
        BUNDLE_ID "${BUNDLE_ID}"
        VERSION "${CURRENT_VERSION}"
        FORMATS ${FORMATS}
        COPY_PLUGIN_AFTER_BUILD ${SHOULD_COPY}
        NEEDS_WEBVIEW2 TRUE
        NEEDS_WEB_BROWSER TRUE
        NEEDS_CURL TRUE)


target_compile_definitions(${BINARY}
        PRIVATE
        VERSION="${CURRENT_VERSION}"
        "DEV_LOCALHOST=$<BOOL:$ENV{DEV_LOCALHOST}>"
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=1
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
        JUCE_VST3_CAN_REPLACE_VST2=0

)

target_sources(${BINARY} PRIVATE ${SOURCES})


#target_compile_options(${BINARY} INTERFACE -Wall -Wextra -Werror)

if (NOT DEV_LOCALHOST AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

    set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
    set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")
    cmake_path(ABSOLUTE_PATH ASSETS_DIR NORMALIZE OUTPUT_VARIABLE PUBLIC_PATH)
    cmake_path(GET PUBLIC_PATH PARENT_PATH WORKING_DIRECTORY)

    add_custom_target(ZipWebViewFiles
            COMMAND
            ${CMAKE_COMMAND} -E tar cvf
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            --format=zip
            "${PUBLIC_PATH}"
            BYPRODUCTS
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            WORKING_DIRECTORY
            "${WORKING_DIRECTORY}"
            COMMENT "Zipping WebView files..."
            VERBATIM
    )

    cmake_path(GET PUBLIC_PATH FILENAME ZIPPED_FILES_PREFIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="${ZIPPED_FILES_PREFIX}/")

    juce_add_binary_data(WebViewFiles
            HEADER_NAME WebViewFiles.h
            NAMESPACE webview_files
            SOURCES ${TARGET_WEBVIEW_FILES_ZIP_PATH}
    )
    add_dependencies(WebViewFiles ZipWebViewFiles)

endif ()

target_link_libraries(
        ${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON} WebViewFiles
        PUBLIC ${MODULE_LINK_CONF})

