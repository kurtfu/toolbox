find_program(GCOV_BIN gcov)
find_program(LCOV_BIN lcov)
find_program(GCOVR_BIN gcovr)

if(NOT GCOV_BIN)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if(NOT LCOV_BIN)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif()

if(NOT GCOVR_BIN)
    message(FATAL_ERROR "gcovr not found! Aborting...")
endif()

add_custom_target(coverage
    COMMAND
        ${CMAKE_COMMAND} -E make_directory docs/coverage
    COMMAND
        ${GCOVR_BIN} --html-details -o docs/coverage/index.html -e .*build.* -e .*tests.*
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    COMMENT
        "Generating code coverage report"
)

function(setup_target_for_coverage target)
    cleanup_coverage_data(${target})
    append_coverage_compiler_flags(${target})
endfunction()

function(cleanup_coverage_data target)
    add_custom_command(
        TARGET
            ${target}
        COMMAND
            ${LCOV_BIN} --zerocounters --directory .
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
    )
endfunction()

function(append_coverage_compiler_flags target)
    target_compile_options(${target}
        PRIVATE
            -fprofile-arcs
            -ftest-coverage
    )

    target_link_options(${target}
        PRIVATE
            --coverage
    )
endfunction()
