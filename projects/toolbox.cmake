find_package(quill CONFIG REQUIRED)
find_package(replxx CONFIG REQUIRED)
find_package(Threads REQUIRED)

find_path(REPLXX_INCLUDE_DIR NAMES "replxx.hxx")

setup_library(toolbox
    SOURCES
        src/utils.cpp
    INCLUDES
        src
        SYSTEM ${REPLXX_INCLUDE_DIR}
    DEPENDENCIES
        quill::quill
        INTERFACE replxx::replxx
        Threads::Threads
)

setup_test(toolbox-test
    SOURCES
        tests/either.cpp
        tests/maybe.cpp
    INCLUDES
        src
    DEPENDENCIES
        Catch2::Catch2WithMain
)
