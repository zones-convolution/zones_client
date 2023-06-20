add_subdirectory(JUCE)

add_library(immer INTERFACE)
target_include_directories(immer SYSTEM INTERFACE immer)

add_library(zug INTERFACE)
target_include_directories(zug SYSTEM INTERFACE zug)

add_library(lager INTERFACE)
target_include_directories(lager SYSTEM INTERFACE lager)
target_link_libraries(lager INTERFACE zug immer Boost::headers)
