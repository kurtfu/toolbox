include(Coverage)
include(Linker)
include(Optimization)
include(Sanitizer)
include(Tests)
include(Warnings)

function(setup_executable target)
    set(oneValueArgs INSTALL)
    set(multiValueArgs SOURCES INCLUDES DEPENDENCIES PROPERTIES DEFINES)

    cmake_parse_arguments(TARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${target})
    _setup_target_includes(${target} PRIVATE)
    _setup_target_dependencies(${target})
    _setup_target_defines(${target} PRIVATE)

    if(DEFINED TARGET_PROPERTIES)
        _setup_target_properties(${target})
    endif()

    setup_target_link_strategy(${target})
    setup_target_optimizations(${target})

    setup_target_warnings(${target})
    setup_target_for_sanitizer(${target})

    if(TARGET_INSTALL)
        install(TARGETS ${target} DESTINATION ${TARGET_INSTALL})
    endif()
endfunction()

function(setup_library target)
    set(oneValueArgs TYPE ALIAS INSTALL)
    set(multiValueArgs SOURCES INCLUDES DEPENDENCIES PROPERTIES DEFINES)

    cmake_parse_arguments(TARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED TARGET_TYPE)
        set(TARGET_TYPE STATIC)
    endif()

    if(TARGET_TYPE STREQUAL "INTERFACE")
        set(TARGET_INCLUDE_SCOPE INTERFACE)
        set(TARGET_DEFINE_SCOPE INTERFACE)
    else()
        set(TARGET_INCLUDE_SCOPE PUBLIC)
        set(TARGET_DEFINE_SCOPE PRIVATE)
    endif()

    _setup_library_sources(${target})
    _setup_target_includes(${target} ${TARGET_INCLUDE_SCOPE})
    _setup_target_dependencies(${target})
    _setup_target_defines(${target} ${TARGET_INCLUDE_SCOPE})

    if(DEFINED TARGET_PROPERTIES)
        _setup_target_properties(${target})
    endif()

    if(NOT TARGET_TYPE STREQUAL "INTERFACE")
        setup_target_link_strategy(${target})
        setup_target_optimizations(${target})

        setup_target_warnings(${target})
        setup_target_for_sanitizer(${target})
    endif()

    if(TARGET_INSTALL)
        install(TARGETS ${target} DESTINATION ${TARGET_INSTALL})
    endif()
endfunction()

function(setup_test target)
    if(NOT ENABLE_TESTS)
        return()
    endif()

    set(multiValueArgs SOURCES INCLUDES DEPENDENCIES PROPERTIES DEFINES)

    cmake_parse_arguments(TARGET "" "" "${multiValueArgs}" ${ARGN})

    _setup_executable_sources(${target})
    _setup_target_includes(${target} PRIVATE)
    _setup_target_dependencies(${target})
    _setup_target_defines(${target} PRIVATE)

    if(DEFINED TARGET_PROPERTIES)
        _setup_target_properties(${target})
    endif()

    setup_target_link_strategy(${target})
    setup_target_optimizations(${target})

    setup_target_warnings(${target})
    setup_target_for_sanitizer(${target})

    catch_discover_tests(${target})

    if(ENABLE_COVERAGE)
        add_coverage(${target})
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
        ${scope}
            ${SYSTEM_INCLUDES}
    )
endmacro()

macro(_setup_target_dependencies target)
    set(DEPENDENCY_DIRECTORIES "")
    set(DEPENDENCY_LIBRARIES "")
    set(DEPENDENCY_INTERFACES "")

    set(IS_DIRECTORY FALSE)
    set(IS_INTERFACE FALSE)

    foreach(token IN LISTS TARGET_DEPENDENCIES)
        if(token STREQUAL "DIRECTORY")
            set(IS_DIRECTORY TRUE)
        elseif(token STREQUAL "INTERFACE")
            set(IS_INTERFACE TRUE)
        else()
            if(IS_DIRECTORY)
                list(APPEND DEPENDENCY_DIRECTORIES ${token})
                set(IS_DIRECTORY FALSE)
            elseif(IS_INTERFACE)
                list(APPEND DEPENDENCY_INTERFACES ${token})
                set(IS_INTERFACE FALSE)
            else()
                list(APPEND DEPENDENCY_LIBRARIES ${token})
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

    target_link_libraries(${target}
        INTERFACE
            ${DEPENDENCY_INTERFACES}
    )
endmacro()

macro(_setup_target_defines target scope)
    target_compile_definitions(${target}
        ${scope}
            ${TARGET_DEFINES}
    )
endmacro()

macro(_setup_target_properties target)
    set_target_properties(${target}
        PROPERTIES
            ${TARGET_PROPERTIES}
    )
endmacro()
