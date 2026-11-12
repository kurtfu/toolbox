option(ENABLE_IPO "Enable Interprocedural Optimizations for project targets" OFF)
option(ENABLE_ARCH_OPTIMIZATIONS "Enable architecture related optimizations" OFF)

set(CLANG_OPTIMIZATIONS
    -march=native
    -mtune=native
)

set(CLANG_OPTIMIZED_CONFIGS
    -ffunction-sections
    -fdata-sections
    $<$<NOT:$<PLATFORM_ID:Apple>>:-fno-plt>
)

set(CLANG_PROFILINGS
    -fno-omit-frame-pointer
    -mno-omit-leaf-frame-pointer
)

set(CLANG_DEBUG_DEFINES
    _GLIBCXX_ASSERTIONS
    _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE
)

set(GCC_OPTIMIZATIONS
    ${CLANG_OPTIMIZATIONS}
)

set(GCC_OPTIMIZED_CONFIGS
    ${CLANG_OPTIMIZED_CONFIGS}
    -fno-semantic-interposition
)

set(GCC_PROFILINGS
    ${CLANG_PROFILINGS}
)

set(GCC_DEBUG_DEFINES
    ${CLANG_DEBUG_DEFINES}
)

get_property(PROJECT_LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
set(PROJECT_IPO_LANGUAGES "")

foreach(language IN ITEMS C CXX)
    if(language IN_LIST PROJECT_LANGUAGES)
        list(APPEND PROJECT_IPO_LANGUAGES ${language})
    endif()
endforeach()

include(CheckSourceCompiles)

function(_check_ipo_supported)
    set(oneValueArgs RESULT OUTPUT)
    set(multiValueArgs LANGUAGES)

    cmake_parse_arguments(TARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(CMAKE_REQUIRED_LINK_OPTIONS -fuse-ld=${PROJECT_LINKER})
    set(CMAKE_REQUIRED_QUIET ON)

    set(${TARGET_RESULT} TRUE PARENT_SCOPE)
    set(${TARGET_OUTPUT} "" PARENT_SCOPE)

    foreach(language IN LISTS TARGET_LANGUAGES)
        if(NOT CMAKE_${language}_COMPILER_AR)
            set(${TARGET_RESULT} FALSE PARENT_SCOPE)
            set(${TARGET_OUTPUT} "LTO-aware archiver not found for ${language}" PARENT_SCOPE)
            return()
        endif()

        list(JOIN CMAKE_${language}_COMPILE_OPTIONS_IPO " " CMAKE_REQUIRED_FLAGS)
        set(CMAKE_REQUIRED_LINK_OPTIONS -fuse-ld=${PROJECT_LINKER} ${CMAKE_${language}_LINK_OPTIONS_IPO} ${CMAKE_${language}_COMPILE_OPTIONS_IPO})

        check_source_compiles(${language}
            "static int f(int x) { return x + 1; } int main(void) { return f(-1); }"
            PROJECT_IPO_SUPPORTED_${language}
        )

        if(NOT PROJECT_IPO_SUPPORTED_${language})
            set(${TARGET_RESULT} FALSE PARENT_SCOPE)
            set(${TARGET_OUTPUT} "Failed to link with ${PROJECT_LINKER}" PARENT_SCOPE)
            return()
        endif()
    endforeach()
endfunction()

if(ENABLE_IPO)
    if(PROJECT_LINKER)
        _check_ipo_supported(RESULT result OUTPUT output LANGUAGES ${PROJECT_IPO_LANGUAGES})
    else()
        include(CheckIPOSupported)
        check_ipo_supported(RESULT result OUTPUT output LANGUAGES ${PROJECT_IPO_LANGUAGES})
    endif()

    if(result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(STATUS "IPO is not supported: ${output}")
    endif()
endif()

if(CMAKE_INTERPROCEDURAL_OPTIMIZATION
    AND CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    AND PROJECT_LINKER STREQUAL "lld")
    if(APPLE)
        set(PROJECT_IPO_LINK_OPTIONS LINKER:-cache_path_lto,${PROJECT_BINARY_DIR}/thinlto-cache)
    else()
        set(PROJECT_IPO_LINK_OPTIONS LINKER:--thinlto-cache-dir=${PROJECT_BINARY_DIR}/thinlto-cache)
    endif()
else()
    set(PROJECT_IPO_LINK_OPTIONS "")
endif()

if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_OPTIMIZED_CONFIGS ${CLANG_OPTIMIZED_CONFIGS})
    set(PROJECT_PROFILINGS ${CLANG_PROFILINGS})
    set(PROJECT_DEBUG_DEFINES ${CLANG_DEBUG_DEFINES})
    set(PROJECT_OPTIMIZATIONS ${CLANG_OPTIMIZATIONS})
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_OPTIMIZED_CONFIGS ${GCC_OPTIMIZED_CONFIGS})
    set(PROJECT_PROFILINGS ${GCC_PROFILINGS})
    set(PROJECT_DEBUG_DEFINES ${GCC_DEBUG_DEFINES})
    set(PROJECT_OPTIMIZATIONS ${GCC_OPTIMIZATIONS})
else()
    if("C" IN_LIST PROJECT_LANGUAGES)
        message(STATUS "No native optimization set for C compiler: '${CMAKE_C_COMPILER_ID}'")
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_OPTIMIZED_CONFIGS ${CLANG_OPTIMIZED_CONFIGS})
    set(PROJECT_PROFILINGS ${CLANG_PROFILINGS})
    set(PROJECT_DEBUG_DEFINES ${CLANG_DEBUG_DEFINES})
    set(PROJECT_OPTIMIZATIONS ${CLANG_OPTIMIZATIONS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_OPTIMIZED_CONFIGS ${GCC_OPTIMIZED_CONFIGS})
    set(PROJECT_PROFILINGS ${GCC_PROFILINGS})
    set(PROJECT_DEBUG_DEFINES ${GCC_DEBUG_DEFINES})
    set(PROJECT_OPTIMIZATIONS ${GCC_OPTIMIZATIONS})
else()
    if("CXX" IN_LIST PROJECT_LANGUAGES)
        message(STATUS "No native optimization set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_optimizations target)
    target_compile_options(${target}
        PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:Release,RelWithDebInfo>>:${PROJECT_OPTIMIZED_CONFIGS}>
        $<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:RelWithDebInfo>>:${PROJECT_PROFILINGS}>
    )

    target_compile_definitions(${target}
        PRIVATE
            $<$<CONFIG:Debug>:${PROJECT_DEBUG_DEFINES}>
    )

    target_link_options(${target}
        PRIVATE
            ${PROJECT_IPO_LINK_OPTIONS}
    )

    if(NOT ENABLE_ARCH_OPTIMIZATIONS)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            $<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<CONFIG:Release,RelWithDebInfo>>:${PROJECT_OPTIMIZATIONS}>
    )
endfunction()
