option(ENABLE_LINTERS "Enable linters for project targets" OFF)

if(NOT ENABLE_LINTERS)
    return()
endif()

find_program(CLANG_TIDY clang-tidy)

if(NOT CLANG_TIDY)
    message(STATUS "clang-tidy not found! Skipping...")
else()
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/.clang-tidy")
        message(WARNING "${PROJECT_SOURCE_DIR}/.clang-tidy not found: clang-tidy will use its default checks")
    endif()

    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY} --extra-arg=-Wno-unknown-warning-option)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=-Wno-unknown-warning-option)
endif()
