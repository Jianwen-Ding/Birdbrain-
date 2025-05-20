# Benchmark cmake, made so that it could be easily excluded in build case

include(FetchContent)

FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.9.4
)

FetchContent_MakeAvailable(google_benchmark)