cmake_minimum_required(VERSION 3.21.0)
project(utils LANGUAGES CXX)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Coverage)
include(CPM)
include(Docs)
include(Linter)
include(Optimization)
include(Utils)

if(DEFINED ENV{VCPKG_ROOT})
    find_package(Catch2)
else()
    CPMAddPackage("gh:catchorg/Catch2#v3.6.0")
endif()

include(CTest)
include(Catch)

setup_executable(
    TARGET
        utils-test
    SOURCES
        tests/circular_buffer.cpp
        tests/either.cpp
    INCLUDES
        include
    LIBRARIES
        Catch2::Catch2WithMain
)

catch_discover_tests(utils-test)
add_coverage(utils-test)
