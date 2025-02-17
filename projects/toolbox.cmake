setup_library(toolbox
    SOURCES
        src/utils.cpp
    INCLUDES
        include
)

setup_executable(toolbox-test
    SOURCES
        tests/either.cpp
        tests/maybe.cpp
    INCLUDES
        include
    DEPENDENCIES
        Catch2::Catch2WithMain
)

catch_discover_tests(toolbox-test)
add_coverage(toolbox-test)
