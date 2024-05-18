option(ENABLE_COVERAGE "Enable code coverage for project targets" OFF)

find_program(GCOV_BIN gcov)
find_program(LCOV_BIN lcov)
find_program(GENHTML_BIN genhtml)

set(Coverage_FOUND TRUE)

if(NOT ENABLE_COVERAGE)
    set(Coverage_FOUND FALSE)
endif()

if(NOT GCOV_BIN)
    message(STATUS "gcov not found! Coverage disabled")
    set(Coverage_FOUND FALSE)
endif()

if(NOT LCOV_BIN)
    message(STATUS "lcov not found! Coverage disabled")
    set(Coverage_FOUND FALSE)
endif()

if(NOT GENHTML_BIN)
    message(STATUS "genhtml not found! Coverage disabled")
    set(Coverage_FOUND FALSE)
endif()

function(add_coverage target)
    if(NOT Coverage_FOUND)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            -fprofile-arcs
            -ftest-coverage
    )

    target_link_options(${target}
        PRIVATE
            --coverage
    )

    add_custom_target("${target}-coverage"
        COMMAND
            ${CMAKE_COMMAND} -E make_directory docs/coverage
        COMMAND
            ${LCOV_BIN} --zerocounters --directory .
        COMMAND
            ${target}
        COMMAND
            ${LCOV_BIN} --capture --no-external --directory . -o docs/coverage/base.info
        COMMAND
            ${GENHTML_BIN} docs/coverage/base.info --output-directory docs/coverage
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
        COMMENT
            "Generating code coverage report"
    )
endfunction()
