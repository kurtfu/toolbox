option(ENABLE_IPO "Enable Interprocedural Optimizations for project targets" OFF)

if(NOT ENABLE_IPO)
    return()
endif()

include(CheckIPOSupported)

check_ipo_supported(RESULT result OUTPUT output)

if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    message(STATUS "IPO is not supported: ${output}")
endif()
