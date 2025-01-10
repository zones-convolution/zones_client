if (DEFINED ENV{CI} AND WIN32)
    execute_process(
            COMMAND nuget install Microsoft.Web.WebView2 -OutputDirectory "${CMAKE_BINARY_DIR}/_deps/Nuget"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif ()

set(JUCE_WEBVIEW2_PACKAGE_LOCATION "${CMAKE_BINARY_DIR}/_deps/Nuget")

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