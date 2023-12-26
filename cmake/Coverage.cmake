find_program(GCOV_BIN gcov)
find_program(LCOV_BIN lcov)
find_program(GENHTML_BIN genhtml)

if(NOT GCOV_BIN)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if(NOT LCOV_BIN)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif()

if(NOT GENHTML_BIN)
    message(FATAL_ERROR "genhtml not found! Aborting...")
endif()

add_custom_target(coverage
    COMMAND
        ${CMAKE_COMMAND} -E make_directory docs/coverage
    COMMAND
        ${LCOV_BIN} --capture --no-external --directory . -o docs/coverage/base.info
    COMMAND
        ${LCOV_BIN} --remove docs/coverage/base.info 'build/*' -o docs/coverage/coverage.info
    COMMAND
        ${GENHTML_BIN} docs/coverage/coverage.info --output-directory docs/coverage
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    COMMENT
        "Generating code coverage report"
)

function(setup_target_for_coverage target)
    _cleanup_coverage_data(${target})
    _append_coverage_compiler_flags(${target})
endfunction()

function(_cleanup_coverage_data target)
    add_custom_command(
        TARGET
            ${target}
        COMMAND
            ${LCOV_BIN} --zerocounters --directory .
        WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}
    )
endfunction()

function(_append_coverage_compiler_flags target)
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
