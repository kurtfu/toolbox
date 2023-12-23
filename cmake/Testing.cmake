find_package(Catch2)

include(Catch)
enable_testing()

function(setup_target_for_testing target)
    target_link_libraries(${target}
        PRIVATE
            Catch2::Catch2WithMain
    )

    catch_discover_tests(${target})
endfunction()
