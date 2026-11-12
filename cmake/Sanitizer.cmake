option(ENABLE_SANITIZERS "Enable code sanitizers for project targets" OFF)

set(SANITIZERS "" CACHE STRING "Sanitizer: address,undefined,thread,leak,memory")

if(ENABLE_SANITIZERS)
    if(NOT SANITIZERS)
        message(FATAL_ERROR "")
    endif()

    if("thread" IN_LIST SANITIZERS AND NOT ("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS))
        message(FATAL_ERROR "ThreadSanitizer cannot be combined with AddressSanitizer/LeakSanitizer")
    endif()

    if("memory" IN_LIST SANITIZERS AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "MemorySanitizer is only supported by Clang")
    endif()

    list(JOIN SANITIZERS "," PROJECT_SANITIZER_LIST)
endif()

function(setup_target_for_sanitizer target)
    if(NOT SANITIZERS)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            -fsanitize=${PROJECT_SANITIZER_LIST}
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
            -g
    )

    target_link_options(${target}
        PRIVATE
            -fsanitize=${PROJECT_SANITIZER_LIST}
    )
endfunction()
