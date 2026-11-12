option(ENABLE_TESTS "Enable unit tests of project modules" OFF)

if(NOT ENABLE_TESTS)
    return()
endif()

find_package(Catch2 CONFIG REQUIRED)

enable_testing()
include(Catch)
