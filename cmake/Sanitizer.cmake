function(setup_target_for_sanitizer target)
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
