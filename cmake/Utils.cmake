include(Linker)
include(Sanitizer)
include(Warnings)

function(setup_executable)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${EXECUTABLE_TARGET} ${EXECUTABLE_SOURCES})
    _setup_target_includes(${EXECUTABLE_TARGET} ${EXECUTABLE_INCLUDES})
    _setup_target_libraries(${EXECUTABLE_TARGET} ${EXECUTABLE_LIBRARIES})

    setup_target_link_strategy(${EXECUTABLE_TARGET})

    setup_target_warnings(${EXECUTABLE_TARGET})
    setup_target_for_sanitizer(${EXECUTABLE_TARGET})
endfunction()

macro(_setup_executable_sources target)
    add_executable(${target}
        ${ARGN}
    )
endmacro()

macro(_setup_target_includes target)
    target_include_directories(${target}
        PRIVATE
            ${ARGN}
    )
endmacro()

macro(_setup_target_libraries target)
    target_link_libraries(${target}
        PRIVATE
            ${ARGN}
    )
endmacro()
