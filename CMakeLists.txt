cmake_minimum_required(VERSION 4.0.0)

project(BirdBrainEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define sources (main.cpp and all .c and .cpp files in the src/ folder)
file(GLOB_RECURSE CPP_SOURCES ./engine/src/*.cpp)
file(GLOB_RECURSE C_SOURCES ./engine/src/*.c)
set(ENGINE_SRC ${CPP_SOURCES} ${C_SOURCES})

# Includes helper files
include(helper.cmake)

# Include directories
glob_directories(ENGINE_INCLUDE engine/include)

# Requires Libraries
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Links Libraries
add_library(ENGINE_INTERFACE INTERFACE)
if(TARGET SDL2::SDL2main)
    target_link_libraries(ENGINE_INTERFACE INTERFACE SDL2::SDL2main dl)
endif()
target_link_libraries(ENGINE_INTERFACE INTERFACE SDL2::SDL2 dl)

message(${CMAKE_CXX_COMPILER_ID})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    target_compile_options(ENGINE_INTERFACE INTERFACE -Winteger-overflow)
endif()

# Combines components together 
combine_target(${PROJECT_NAME} engine/main.cpp "${ENGINE_SRC}" "${ENGINE_INCLUDE}" "ENGINE_INTERFACE")

include(CTest)
add_subdirectory(./tests)