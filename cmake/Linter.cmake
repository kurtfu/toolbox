option(ENABLE_LINTERS "Enable linters for project targets" OFF)

if(NOT ENABLE_LINTERS)
    return()
endif()

find_program(CLANG_TIDY clang-tidy)
find_program(CPPCHECK cppcheck)
find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)

if(NOT CLANG_TIDY)
    message(STATUS "clang-tidy not found! Skipping...")
else()
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
endif()

if(NOT CPPCHECK)
    message(STATUS "cppcheck not found! Skipping...")
else()
    set(CMAKE_C_CPPCHECK ${CPPCHECK} --enable=all --suppress=missingIncludeSystem)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=all --suppress=missingIncludeSystem)
endif()

if(NOT INCLUDE_WHAT_YOU_USE)
    message(STATUS "include-what-you-use not found! Skipping...")
else()
    file(WRITE ${CMAKE_BINARY_DIR}/.clang-tidy "Checks: -*,readability-function-size\n")

    set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
endif()
