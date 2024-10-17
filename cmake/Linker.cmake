option(USE_STATIC_STD "Use static version of standard libraries" OFF)

set(CXX_LINK_FLAGS_STATIC_STD
    -static-libgcc
    -static-libstdc++
)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang|GNU")
    if(USE_STATIC_STD)
        set(PROJECT_CXX_LINK_OPTIONS ${CXX_LINK_FLAGS_STATIC_STD})
    endif()
else()
    if("CXX" IN_LIST languages)
        message(STATUS "No linking option set for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

function(setup_target_link_strategy target)
    target_link_options(${target}
        PRIVATE
            ${PROJECT_CXX_LINK_OPTIONS}
    )
endfunction()
