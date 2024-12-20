include(Linker)
include(Sanitizer)
include(Warnings)

function(setup_executable target)
    set(multiValueArgs SOURCES INCLUDES DEPENDENCIES INSTALL PROPERTIES)

    cmake_parse_arguments(TARGET "" "" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${target})
    _setup_target_includes(${target} PUBLIC)
    _setup_target_dependencies(${target})

    if (DEFINED TARGET_PROPERTIES)
        _setup_target_properties(${target})
    endif()

    setup_target_link_strategy(${target})

    setup_target_warnings(${target})
    setup_target_for_sanitizer(${target})

    if(TARGET_INSTALL)
        install(TARGETS ${target} DESTINATION ${TARGET_INSTALL})
    endif()
endfunction()

function(setup_library target)
    set(multiValueArgs TYPE SOURCES INCLUDES DEPENDENCIES INSTALL PROPERTIES)

    cmake_parse_arguments(TARGET "" "" "${multiValueArgs}" ${ARGN})

    _setup_library_sources(${target})
    _setup_target_includes(${target} PRIVATE)
    _setup_target_dependencies(${target})

    if (DEFINED TARGET_PROPERTIES)
        _setup_target_properties(${target})
    endif()

    setup_target_link_strategy(${target})

    setup_target_warnings(${target})
    setup_target_for_sanitizer(${target})

    if(TARGET_INSTALL)
        install(TARGETS ${target} DESTINATION ${TARGET_INSTALL})
    endif()
endfunction()

macro(_setup_executable_sources target)
    add_executable(${target}
        ${TARGET_SOURCES}
    )
endmacro()

macro(_setup_library_sources target type)
    add_library(${target} ${type}
        ${TARGET_SOURCES}
    )
endmacro()

macro(_setup_target_includes target scope)
    set(REGULAR_INCLUDES "")
    set(SYSTEM_INCLUDES "")

    set(IS_SYSTEM FALSE)

    foreach(token IN LISTS TARGET_INCLUDES)
        if(token STREQUAL "SYSTEM")
            set(IS_SYSTEM TRUE)
        else()
            if(NOT IS_SYSTEM)
                list(APPEND REGULAR_INCLUDES ${token})
            else()
                list(APPEND SYSTEM_INCLUDES ${token})
                set(IS_SYSTEM FALSE)
            endif()
        endif()
    endforeach()

    target_include_directories(${target}
        ${scope}
            ${REGULAR_INCLUDES}
    )

    target_include_directories(${target} SYSTEM
        PRIVATE
            ${SYSTEM_INCLUDES}
    )
endmacro()

macro(_setup_target_dependencies target)
    target_link_libraries(${target}
        PRIVATE
            ${TARGET_DEPENDENCIES}
    )
endmacro()

function(_setup_target_properties target)
    set_target_properties(${target}
        PROPERTIES
            ${TARGET_PROPERTIES}
    )
endfunction()
