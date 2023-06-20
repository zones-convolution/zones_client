add_subdirectory(JUCE)

add_library(immer INTERFACE)
target_include_directories(immer INTERFACE immer)

add_library(zug INTERFACE)
target_include_directories(zug INTERFACE zug)

add_library(lager INTERFACE)
target_include_directories(lager INTERFACE lager)
target_link_libraries(lager INTERFACE zug immer Boost::headers)