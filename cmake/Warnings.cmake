option(ENABLE_WARNINGS "Enable compiler warnings for project targets" OFF)

set(MSVC_WARNINGS
    /W4
    /w14242
    /w14254
    /w14263
    /w14265
    /w14287
    /w14296
    /w14311
    /w14555
    /w14826
    /w14928
)

set(COMMON_C_WARNINGS
    -Wall
    -Wextra
    -Wcast-qual
    -Wconversion
    -Wdouble-promotion
    -Wformat=2
    -Wformat-security
    -Wimplicit-fallthrough
    -Wnull-dereference
    -Wpedantic
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wunused
)

set(CLANG_C_WARNINGS
    ${COMMON_C_WARNINGS}
    -Wcast-align
    -Wextra-semi
)

set(CLANG_CXX_WARNINGS
    ${CLANG_C_WARNINGS}
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wzero-as-null-pointer-constant
)

set(GCC_C_WARNINGS
    ${COMMON_C_WARNINGS}
    -Wcast-align=strict
    -Wduplicated-cond
    -Wduplicated-branches
    -Wextra-semi
    -Wlogical-op
)

set(GCC_CXX_WARNINGS
    ${GCC_C_WARNINGS}
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wuseless-cast
    -Wzero-as-null-pointer-constant
)

get_property(PROJECT_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)

if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_C_WARNINGS ${CLANG_C_WARNINGS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_C_WARNINGS ${GCC_C_WARNINGS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(PROJECT_C_WARNINGS ${MSVC_WARNINGS})
else()
    if("C" IN_LIST PROJECT_LANGUAGES)
        message(STATUS "No compiler warnings set for C compiler: '${CMAKE_C_COMPILER_ID}'")
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_CXX_WARNINGS ${CLANG_CXX_WARNINGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_WARNINGS ${GCC_CXX_WARNINGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(PROJECT_CXX_WARNINGS ${MSVC_WARNINGS})
else()
    if("CXX" IN_LIST PROJECT_LANGUAGES)
        message(STATUS "No compiler warnings set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_warnings target)
    if(NOT ENABLE_WARNINGS)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:C>:${PROJECT_C_WARNINGS}>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_CXX_WARNINGS}>
    )
endfunction()
