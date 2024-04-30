add_subdirectory(JUCE)

add_library(immer INTERFACE)
target_include_directories(immer SYSTEM INTERFACE immer)

add_library(zug INTERFACE)
target_include_directories(zug SYSTEM INTERFACE zug)

add_library(lager INTERFACE)
target_include_directories(lager SYSTEM INTERFACE lager)
target_link_libraries(lager INTERFACE zug immer Boost::headers)

add_library(choc INTERFACE)
target_include_directories(choc SYSTEM INTERFACE choc)

add_library(stretch INTERFACE)
target_include_directories(stretch SYSTEM INTERFACE stretch)

add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE glm)

add_library(tinycolormap INTERFACE)
target_include_directories(tinycolormap SYSTEM INTERFACE tinycolormap/include)

juce_add_module(melatonin_test_helpers)
juce_add_module(zones_convolver/zones_convolver)

add_subdirectory(JUCE/extras/AudioPluginHost)
