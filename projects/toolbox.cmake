find_package(quill)
find_package(replxx)
find_package(Threads)

find_path(REPLXX_INCLUDE_DIR NAMES "replxx.hxx")

setup_library(toolbox
    SOURCES
        src/utils.cpp
    INCLUDES
        include
        SYSTEM ${REPLXX_INCLUDE_DIR}
    DEPENDENCIES
        quill::quill
        replxx::replxx
        Threads::Threads
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
