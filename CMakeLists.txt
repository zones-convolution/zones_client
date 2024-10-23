cmake_minimum_required(VERSION 3.20)
project(zones VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

set(PROJECT_NAME "zones")
set(PRODUCT_NAME "Zones")
set(COMPANY_NAME "Zones")
set(BUNDLE_ID "com.zones.zones")
set(CURRENT_VERSION "1.0.0")

add_subdirectory(lib)
add_subdirectory(modules)
add_subdirectory(client)

include(cmake/github_env.cmake)