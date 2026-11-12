option(ENABLE_COVERAGE "Enable code coverage for project targets" OFF)

find_program(GCOV_BIN gcov)
find_program(LCOV_BIN lcov)
find_program(GENHTML_BIN genhtml)

set(COVERAGE_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/docs/coverage")
set(COVERAGE_EXCLUDES "*/catch2/*;*/tests/*;*/vcpkg_installed/*")

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
            --coverage
    )

    target_link_options(${target}
        PRIVATE
            --coverage
    )

    add_custom_target("${target}-coverage"
        COMMAND
            ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_DIR}
        COMMAND
            ${LCOV_BIN} --zerocounters --directory ${PROJECT_BINARY_DIR}
        COMMAND
            $<TARGET_FILE:${target}>
        COMMAND
            ${LCOV_BIN} --capture --no-external
                --directory ${PROJECT_BINARY_DIR}
                --gcov-tool ${GCOV_BIN}
                --base-directory ${PROJECT_SOURCE_DIR}
                -o ${COVERAGE_OUTPUT_DIR}/base.info
        COMMAND
            ${LCOV_BIN} --remove ${COVERAGE_OUTPUT_DIR}/base.info ${COVERAGE_EXCLUDES}
                -o ${COVERAGE_OUTPUT_DIR}/filtered.info
        COMMAND
            ${GENHTML_BIN} ${COVERAGE_OUTPUT_DIR}/filtered.info --output-directory ${COVERAGE_OUTPUT_DIR}
        WORKING_DIRECTORY
            ${PROJECT_BINARY_DIR}
        DEPENDS
            ${target}
        COMMENT
            "Generating code coverage report"
        VERBATIM
    )
endfunction()
