option(ENABLE_LINTERS "Enable linters for project targets" OFF)

if(NOT ENABLE_LINTERS)
    return()
endif()

find_program(CLANG_TIDY clang-tidy)

if(NOT CLANG_TIDY)
    message(STATUS "clang-tidy not found! Skipping...")
else()
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
endif()
