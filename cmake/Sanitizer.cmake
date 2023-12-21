option(ENABLE_SANITIZERS "Enable code sanitizers for project targets" OFF)

function(setup_target_for_sanitizer target)
    if(NOT ENABLE_SANITIZERS)
        return()
    endif()

    target_compile_options(${target}
        PRIVATE
            -fsanitize=address
            -fsanitize=undefined
            -fno-sanitize-recover=all
    )

    target_link_options(${target}
        PRIVATE
            -fsanitize=address
            -fsanitize=undefined
    )
endfunction()
