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

update_from_git(immer "https://github.com/arximboldi/immer"
        "0b3aaf699b9d6f2e89f8e2b6d1221c307e02bda3")
add_library(immer INTERFACE)
target_include_directories(immer INTERFACE "${immer_SOURCE_DIR}")

update_from_git(zug "https://github.com/arximboldi/zug"
        "d7e814b45fceceee3cb1442997d8b46cee4764ec")
add_library(zug INTERFACE)
target_include_directories(zug INTERFACE "${zug_SOURCE_DIR}")

update_from_git(lager "https://github.com/arximboldi/lager"
        "8fd338d6d67dd2efc4f38c0634c2f849a7db8774")
add_library(lager INTERFACE)
target_include_directories(lager INTERFACE "${lager_SOURCE_DIR}")
target_link_libraries(lager INTERFACE zug immer Boost::headers)

find_package(Catch2 3 REQUIRED)
find_package(Boost 1.82.0)

add_subdirectory(modules)
add_subdirectory(client)