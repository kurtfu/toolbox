option(ENABLE_SANITIZERS "Enable code sanitizers for project targets" OFF)

set(SANITIZERS "" CACHE STRING "Sanitizer: address,undefined,thread,leak,memory")

if(ENABLE_SANITIZERS)
    if(NOT SANITIZERS)
        message(FATAL_ERROR "At least one sanitizer must be specified")
    endif()

    if("thread" IN_LIST SANITIZERS AND ("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS))
        message(FATAL_ERROR "ThreadSanitizer cannot be combined with AddressSanitizer/LeakSanitizer")
    endif()

    if("memory" IN_LIST SANITIZERS AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "MemorySanitizer is only supported by Clang")
    endif()

    list(JOIN SANITIZERS "," PROJECT_SANITIZERS)
endif()

function(setup_target_for_sanitizer target)
    if(NOT ENABLE_SANITIZERS)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:C,CXX>:-fsanitize=${PROJECT_SANITIZERS}>
            $<$<COMPILE_LANGUAGE:C,CXX>:-fno-sanitize-recover=all>
            $<$<COMPILE_LANGUAGE:C,CXX>:-fno-omit-frame-pointer>
            $<$<COMPILE_LANGUAGE:C,CXX>:-g>
    )

    target_link_options(${target}
        PRIVATE
            $<$<COMPILE_LANGUAGE:C,CXX>:-fsanitize=${PROJECT_SANITIZERS}>
    )
endfunction()
