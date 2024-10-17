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

set(CLANG_C_WARNINGS
    -Wall
    -Wextra
    -Wcast-align
    -Wconversion
    -Wdouble-promotion
    -Wformat-security
    -Wimplicit-fallthrough
    -Wnull-dereference
    -Wpedantic
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wunused
)

set(CLANG_CXX_WARNINGS
    ${CLANG_C_WARNINGS}
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnon-virtual-dtor
)

set(GCC_C_WARNINGS
    ${CLANG_C_WARNINGS}
)

set(GCC_CXX_WARNINGS
    ${CLANG_CXX_WARNINGS}
    -Wduplicated-branches
    -Wduplicated-cond
    -Wlogical-op
    -Wuseless-cast
)

if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_C_WARNINGS ${CLANG_C_WARNINGS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_C_WARNINGS ${GCC_C_WARNINGS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(PROJECT_C_WARNINGS ${MSVC_WARNINGS})
else()
    if("C" IN_LIST languages)
        message(STATUS "No compiler warnigs set for C compiler: '${CMAKE_C_COMPILER_ID}'")
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_CXX_WARNINGS ${CLANG_CXX_WARNINGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_WARNINGS ${GCC_CXX_WARNINGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(PROJECT_CXX_WARNINGS ${MSVC_WARNINGS})
else()
    if("CXX" IN_LIST languages)
        message(STATUS "No compiler warnigs set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_warnings target)
    target_compile_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:C>:${PROJECT_C_WARNINGS}>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_CXX_WARNINGS}>
    )
endfunction()
