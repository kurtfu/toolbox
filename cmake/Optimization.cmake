option(ENABLE_IPO "Enable Interprocedural Optimizations for project targets" OFF)
option(OPTIMIZE_FOR_NATIVE "Build for native architecture" OFF)

include(CheckIPOSupported)

check_ipo_supported(RESULT result OUTPUT output)

if(result)
    if(ENABLE_IPO)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
else()
    message(STATUS "IPO is not supported: ${output}")
endif()

set(CLANG_C_OPTIMIZATIONS
    -march=native
    -mtune=native
)

set(CLANG_CXX_OPTIMIZATIONS
    ${CLANG_C_OPTIMIZATIONS}
)

set(GCC_C_OPTIMIZATIONS
    ${CLANG_C_OPTIMIZATIONS}
)

set(GCC_CXX_OPTIMIZATIONS
    ${CLANG_CXX_OPTIMIZATIONS}
)

if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_C_OPTIMIZATIONS ${CLANG_C_OPTIMIZATIONS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_C_OPTIMIZATIONS ${GCC_C_OPTIMIZATIONS})
else()
    if("C" IN_LIST languages)
        message(STATUS "No native optimization set for C compiler: '${CMAKE_C_COMPILER_ID}'")
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_CXX_OPTIMIZATIONS ${CLANG_CXX_OPTIMIZATIONS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_OPTIMIZATIONS ${GCC_CXX_OPTIMIZATIONS})
else()
    if("CXX" IN_LIST languages)
        message(STATUS "No native optimization set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_optimizations target)
    if(NOT OPTIMIZE_FOR_NATIVE)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:${PROJECT_C_OPTIMIZATIONS}>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Release>>:${PROJECT_CXX_OPTIMIZATIONS}>
    )
endfunction()
