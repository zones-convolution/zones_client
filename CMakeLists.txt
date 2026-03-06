cmake_minimum_required(VERSION 3.20)
project(zones VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CATCH_INSTALL_DOCS OFF)
set(CATCH_INSTALL_EXTRAS OFF)
set(BUILD_TESTING OFF)

find_package(Catch2 3 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include(CTest)
include(Catch)

include(cmake/update_from_git.cmake)
include(cmake/zones_module_common.cmake)
include(cmake/embed_resources.cmake)

execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
        COMMAND git describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE CURRENT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(PROJECT_NAME "zones_client")
set(PRODUCT_NAME "Zones")
set(COMPANY_NAME "Zones")
set(BUNDLE_ID "com.zones.zones")

add_subdirectory(lib)
add_subdirectory(modules)
add_subdirectory(client)

include(cmake/github_env.cmake)