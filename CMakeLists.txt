cmake_minimum_required(VERSION 3.30.4)

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
    ${CMAKE_SOURCE_DIR}/engine/include/rendering/SFML
    ${CMAKE_SOURCE_DIR}/engine/include/input
    ${CMAKE_SOURCE_DIR}/engine/include/math
    ${CMAKE_SOURCE_DIR}/engine/include/ecs
    ${CMAKE_SOURCE_DIR}/engine/include/ecs/components
)

# Link libraries
# find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)
# target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)

include(CTest)
add_subdirectory(./tests)