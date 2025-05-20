# Benchmark cmake, made so that it could be easily excluded in build case

include(FetchContent)

FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.9.4
)

FetchContent_MakeAvailable(google_benchmark)

add_library(BENCH_INTERFACE INTERFACE)
target_link_libraries(
  BENCH_INTERFACE INTERFACE
  benchmark::benchmark
  BIRDBRAIN_INTERFACE)

add_executable (Bench "gbench.cpp")
target_link_libraries(Bench PRIVATE "BENCH_INTERFACE")