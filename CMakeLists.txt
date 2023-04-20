cmake_minimum_required(VERSION 3.18.0)
project(utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(codecov)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "(Clang|GNU)")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(utils-test
    tests/circular_buffer.cpp
)

target_link_libraries(utils-test
    PRIVATE
        Catch2::Catch2WithMain
)

catch_discover_tests(utils-test)
add_coverage(utils-test)
