include(Linker)
include(Sanitizer)
include(Warnings)

function(setup_executable target)
    set(multiValueArgs SOURCES INCLUDES DEPENDENCIES INSTALL PROPERTIES DEFINES)

    cmake_parse_arguments(TARGET "" "" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${target})
    _setup_target_includes(${target} PUBLIC)
    _setup_target_dependencies(${target})
    _setup_target_defines(${target})

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
    set(multiValueArgs TYPE SOURCES INCLUDES DEPENDENCIES INSTALL PROPERTIES DEFINES ALIAS)

    cmake_parse_arguments(TARGET "" "" "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED TARGET_TYPE)
        set(TARGET_TYPE STATIC)
    endif()

    _setup_library_sources(${target})
    _setup_target_includes(${target} PRIVATE)
    _setup_target_dependencies(${target})
    _setup_target_defines(${target})

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

macro(_setup_library_sources target)
    add_library(${target} ${TARGET_TYPE}
        ${TARGET_SOURCES}
    )

    if(DEFINED TARGET_ALIAS)
        add_library(${TARGET_ALIAS} ALIAS ${target})
    endif()
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
    set(DEPENDENCY_DIRECTORIES "")
    set(DEPENDENCY_LIBRARIES "")

    set(IS_DIRECTORY FALSE)

    foreach(token IN LISTS TARGET_DEPENDENCIES)
        if(token STREQUAL "DIRECTORY")
            set(IS_DIRECTORY TRUE)
        else()
            if(NOT IS_DIRECTORY)
                list(APPEND DEPENDENCY_LIBRARIES ${token})
            else()
                list(APPEND DEPENDENCY_DIRECTORIES ${token})
                set(IS_DIRECTORY FALSE)
            endif()
        endif()
    endforeach()

    target_link_directories(${target}
        PRIVATE
            ${DEPENDENCY_DIRECTORIES}
    )

    target_link_libraries(${target}
        PRIVATE
            ${DEPENDENCY_LIBRARIES}
    )
endmacro()

macro(_setup_target_defines target)
    target_compile_definitions(${target}
        PRIVATE
            ${TARGET_DEFINES}
    )
endmacro()

macro(_setup_target_properties target)
    set_target_properties(${target}
        PROPERTIES
            ${TARGET_PROPERTIES}
    )
endmacro()
