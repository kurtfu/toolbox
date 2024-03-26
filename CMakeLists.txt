cmake_minimum_required(VERSION 3.21.0)
project(utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Docs)
include(Linter)
include(Optimization)
include(Utils)

setup_test(
    TARGET
        utils-test
    SOURCES
        tests/circular_buffer.cpp
        tests/either.cpp
    INCLUDES
        ${CMAKE_SOURCE_DIR}/include
)
