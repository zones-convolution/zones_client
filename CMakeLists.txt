cmake_minimum_required(VERSION 3.20)
project(zones VERSION 1.0)

include(cmake/update_from_git.cmake)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
set(BUILD_TESTING OFF)

include(cmake/update_from_git.cmake)
include(cmake/zones_module_common.cmake)

update_from_git(juce "https://github.com/juce-framework/JUCE"
        "2b16c1b94c90d0db3072f6dc9da481a9484d0435")
add_subdirectory(${juce_SOURCE_DIR})

update_from_git(catch2 "https://github.com/catchorg/Catch2"
        "97c48e0c343d26d50764fafdc90b1e630fbd10ce")
add_subdirectory(${catch2_SOURCE_DIR})

#add_library(common INTERFACE)
#link_libraries(common)

add_subdirectory(modules)
add_subdirectory(client)