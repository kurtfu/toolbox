cmake_minimum_required(VERSION 3.18.0)
project(utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Coverage)
find_package(Docs)
find_package(Testing)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(utils-test
    tests/circular_buffer.cpp
)

setup_target_for_coverage(utils-test)
setup_target_for_testing(utils-test)
