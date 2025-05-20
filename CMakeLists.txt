cmake_minimum_required(VERSION 4.0.0)

project(BirdBrainEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Includes helper files
include(helper.cmake)

add_subdirectory(./core)

# Combines components together 
combine_target(${PROJECT_NAME} core/main.cpp "BIRDBRAIN_INTERFACE")

# Adds tests if wanted
include(CTest)
add_subdirectory(./tests)

# Adds benchmarks if wanted
add_subdirectory(./benchmarks)