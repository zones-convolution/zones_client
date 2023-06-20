set(MODULE_LINK_CONF
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

function(zones_add_module)
    cmake_parse_arguments(
            PARSED_ARGS
            ""
            "MODULE"
            "SOURCES;DEPS"
            ${ARGN}
    )

    if (NOT PARSED_ARGS_MODULE)
        message(FATAL_ERROR "You must provide a module name")
    endif (NOT PARSED_ARGS_MODULE)

    add_library(${PARSED_ARGS_MODULE} STATIC ${PARSED_ARGS_SOURCES})
    target_include_directories(${PARSED_ARGS_MODULE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(
            ${PARSED_ARGS_MODULE}
            PUBLIC ${MODULE_LINK_CONF} ${PARSED_ARGS_DEPS})
endfunction(zones_add_module)

function(zones_add_test)
    cmake_parse_arguments(
            PARSED_ARGS
            ""
            "MODULE"
            "SOURCES;DEPS"
            ${ARGN}
    )

    set(BINARY_TST ${PARSED_ARGS_MODULE}_tests)
    add_executable(${BINARY_TST})
    target_sources(${BINARY_TST} PRIVATE ${PARSED_ARGS_SOURCES})
    target_link_libraries(
            ${BINARY_TST}
            PRIVATE ${PARSED_ARGS_DEPS} Catch2::Catch2WithMain
            PUBLIC ${MODULE_LINK_CONF})
endfunction(zones_add_test)
