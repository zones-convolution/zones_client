zones_add_module(
        MODULE spectrogram_visualiser
        SOURCES src/spectrogram_visualiser/SpectrogramVisualiserComponent.h src/spectrogram_visualiser/SpectrogramVisualiserComponent.cpp
        DEPS juce::juce_gui_basics juce::juce_opengl zones_look_and_feel
)

# https://github.com/shir0areed/non-invasive-embed-resources.cmake
# https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake

function(embed_resources target)
    set(script_path "${CMAKE_CURRENT_BINARY_DIR}/anything_to_c.cmake")
    file(WRITE "${script_path}" "file(READ \${CMAKE_ARGV3} buf HEX)\n")
    file(APPEND "${script_path}" "string(REGEX REPLACE \"([0-9a-f][0-9a-f])\" \"0x\\\\1, \" buf \${buf})\n")
    file(APPEND "${script_path}" "file(WRITE \${CMAKE_ARGV4} \"const unsigned char \${CMAKE_ARGV5}[] = { \${buf}0x00 };\\n\")\n")
    file(APPEND "${script_path}" "file(APPEND \${CMAKE_ARGV4} \"const unsigned \${CMAKE_ARGV6} = sizeof(\${CMAKE_ARGV5}) - 1;\\n\")\n")
    foreach (res_path ${ARGN})
        string(MAKE_C_IDENTIFIER ${res_path} identifier)
        set(src_path "${CMAKE_CURRENT_SOURCE_DIR}/${res_path}")
        set(dst_path "${CMAKE_CURRENT_BINARY_DIR}/${identifier}.c")
        set(anything_to_c ${CMAKE_COMMAND} -P ${script_path} ${src_path} ${dst_path} ${identifier} ${identifier}_size)
        add_custom_command(OUTPUT ${dst_path} COMMAND ${anything_to_c} DEPENDS ${src_path} VERBATIM)
        target_sources(${target} PRIVATE ${src_path} ${dst_path})
    endforeach ()
endfunction()

embed_resources(spectrogram_visualiser
        shaders/test.frag.glsl shaders/test.vert.glsl)