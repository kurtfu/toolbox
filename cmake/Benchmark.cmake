option(ENABLE_BENCHMARKS "Enable micro benchmarks of project modules" OFF)

if(ENABLE_BENCHMARKS)
    find_package(benchmark CONFIG REQUIRED)
endif()
