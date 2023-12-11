find_program(GCOV_BIN gcov)
find_program(LCOV_BIN lcov)

if(NOT GCOV_BIN)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if(NOT LCOV_BIN)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif()

function(setup_target_for_coverage target)
    cleanup_coverage_data(${target})
    append_coverage_compiler_flags(${target})
endfunction()

function(cleanup_coverage_data target)
    add_custom_command(
        TARGET
            ${target}
        COMMAND
            ${LCOV_BIN} --gcov-tool ${GCOV_BIN} -directory . -b ${PROJECT_SOURCE_DIR} --zerocounters
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
