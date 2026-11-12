set(PROJECT_COMMON_LINK_OPTIONS
    $<$<PLATFORM_ID:Linux>:LINKER:-z,now>
    $<$<AND:$<PLATFORM_ID:Linux>,$<CONFIG:Release,RelWithDebInfo>>:LINKER:--gc-sections>
)

if(PROJECT_LINKER)
    list(APPEND PROJECT_COMMON_LINK_OPTIONS -fuse-ld=${PROJECT_LINKER})
endif()

function(setup_target_link_strategy target)
    target_link_options(${target}
        PRIVATE
            ${PROJECT_COMMON_LINK_OPTIONS}
    )
endfunction()
