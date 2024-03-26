include(Coverage)
include(Sanitizer)
include(Testing)
include(Warnings)

function(setup_executable)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${EXECUTABLE_TARGET} ${EXECUTABLE_SOURCES})
    _setup_target_includes(${EXECUTABLE_TARGET} ${EXECUTABLE_INCLUDES})
    _setup_target_libraries(${EXECUTABLE_TARGET} ${EXECUTABLE_LIBRARIES})

    setup_target_warnings(${EXECUTABLE_TARGET})
    setup_target_for_sanitizer(${EXECUTABLE_TARGET})
endfunction()

function(setup_test)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${EXECUTABLE_TARGET} ${EXECUTABLE_SOURCES})
    _setup_target_includes(${EXECUTABLE_TARGET} ${EXECUTABLE_INCLUDES})
    _setup_target_libraries(${EXECUTABLE_TARGET} ${EXECUTABLE_LIBRARIES})

    setup_target_warnings(${EXECUTABLE_TARGET})

    setup_target_for_coverage(${EXECUTABLE_TARGET})
    setup_target_for_sanitizer(${EXECUTABLE_TARGET})
    setup_target_for_testing(${EXECUTABLE_TARGET})
endfunction()

function(_setup_executable_sources target)
    add_executable(${target}
        ${ARGN}
    )
endfunction()

function(_setup_target_includes target)
    target_include_directories(${target}
        PRIVATE
            ${ARGN}
    )
endfunction()

function(_setup_target_libraries target)
    target_link_libraries(${target}
        PRIVATE
            ${ARGN}
    )
endfunction()
