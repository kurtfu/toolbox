cmake_minimum_required(VERSION 3.18.0)
project(utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)

if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    message(STATUS "IPO is not supported: ${output}")
endif()

include(Coverage)
include(Docs)
include(Linter)
include(Sanitizer)
include(Testing)
include(Warnings)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(utils-test
    tests/circular_buffer.cpp
    tests/result.cpp
)

setup_target_for_coverage(utils-test)
setup_target_for_sanitizer(utils-test)
setup_target_for_testing(utils-test)
