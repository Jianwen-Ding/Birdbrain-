cmake_minimum_required(VERSION 4.0.0)

project(BirdBrainEngine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define sources (main.cpp and all .c and .cpp files in the src/ folder)
file(GLOB_RECURSE CPP_SOURCES ./engine/src/*.cpp)
file(GLOB_RECURSE C_SOURCES ./engine/src/*.c)

# actually create executable
add_executable(${PROJECT_NAME} engine/main.cpp ${CPP_SOURCES} ${C_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/engine/include/rendering
    ${CMAKE_SOURCE_DIR}/engine/include/rendering/SDL
    ${CMAKE_SOURCE_DIR}/engine/include/input
    ${CMAKE_SOURCE_DIR}/engine/include/math
    ${CMAKE_SOURCE_DIR}/engine/include/ecs
    ${CMAKE_SOURCE_DIR}/engine/include/ecs/components
)

# Link libraries
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

if(TARGET SDL2::SDL2main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main dl)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 dl)

message(${CMAKE_CXX_COMPILER_ID})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    target_compile_options(${PROJECT_NAME} PUBLIC -Winteger-overflow)
endif()

include(CTest)
add_subdirectory(./tests)