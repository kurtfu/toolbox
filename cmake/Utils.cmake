include(Linker)
include(Sanitizer)
include(Warnings)

set(CLANG_CXX_MISC_OPTIONS
    -save-temps
)

set(GCC_CXX_MISC_OPTIONS
    ${CLANG_CXX_MISC_OPTIONS}
)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_CXX_MISC_OPTIONS ${CLANG_CXX_MISC_OPTIONS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_MISC_OPTIONS ${GCC_CXX_MISC_OPTIONS})
else()
    if("CXX" IN_LIST languages)
        message(STATUS "No extra options set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_executable)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES INCLUDES LIBRARIES)

    cmake_parse_arguments(EXECUTABLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${EXECUTABLE_TARGET} ${EXECUTABLE_SOURCES})
    _setup_target_includes(${EXECUTABLE_TARGET} ${EXECUTABLE_INCLUDES})
    _setup_target_libraries(${EXECUTABLE_TARGET} ${EXECUTABLE_LIBRARIES})
    _setup_misc_options(${EXECUTABLE_TARGET})

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

macro(_setup_misc_options target)
    target_compile_options(${target}
        PRIVATE
            ${PROJECT_CXX_MISC_OPTIONS}
    )
endmacro()
