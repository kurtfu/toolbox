option(TOOLBOX_BUILD_TESTS "Build project tests" OFF)
option(TOOLBOX_BUILD_BENCHMARKS "Build project benchmarks" OFF)

find_package(quill CONFIG REQUIRED)
find_package(replxx CONFIG REQUIRED)
find_package(Threads REQUIRED)

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

if(TOOLBOX_BUILD_TESTS)
    setup_test(toolbox-test
        SOURCES
            tests/either.cpp
            tests/maybe.cpp
        INCLUDES
            src
        DEPENDENCIES
            Catch2::Catch2WithMain
    )
endif()
