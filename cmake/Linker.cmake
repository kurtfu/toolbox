option(USE_STATIC_STD "Use static version of standard libraries" OFF)

set(GCC_C_LINK_FLAGS
    -static-libgcc
)

set(GCC_CXX_LINK_FLAGS
    -static-libgcc
    -static-libstdc++
)

set(CLANG_CXX_LINK_FLAGS
    -static-libstdc++
)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_C_LINK_OPTIONS ${GCC_C_LINK_FLAGS})
else()
    if("C" IN_LIST languages)
        message(STATUS "No compiler warnigs set for C compiler: '${CMAKE_C_COMPILER_ID}'")
    endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(PROJECT_CXX_LINK_OPTIONS ${CLANG_CXX_LINK_FLAGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PROJECT_CXX_LINK_OPTIONS ${GCC_CXX_LINK_FLAGS})
else()
    if("CXX" IN_LIST languages)
        message(STATUS "No linking option set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_link_strategy target)
    if(NOT USE_STATIC_STD)
        return()
    endif()

    target_link_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:C>:${PROJECT_C_LINK_OPTIONS}>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_CXX_LINK_OPTIONS}>
    )
endfunction()
