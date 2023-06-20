cmake_minimum_required(VERSION 3.20)
project(zones VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
set(BUILD_TESTING OFF)

include(cmake/update_from_git.cmake)
include(cmake/zones_module_common.cmake)

find_package(Catch2 3 REQUIRED)
find_package(Boost 1.82.0)
add_subdirectory(lib)

add_subdirectory(modules)
add_subdirectory(client)
