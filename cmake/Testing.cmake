find_package(Catch2)

if(Catch2_FOUND)
    include(Catch)
    enable_testing()
endif()

function(setup_target_for_testing target)
    if(NOT Catch2_FOUND)
        return()
    endif()

    target_link_libraries(${target}
        PRIVATE
            Catch2::Catch2WithMain
    )

    catch_discover_tests(${target})
endfunction()
