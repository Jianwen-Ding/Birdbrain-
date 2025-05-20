
# Defines dependencies for Birdbrain specific core code
# Define sources (main.cpp and all .c and .cpp files in the src/ folder)
file(GLOB_RECURSE CPP_SOURCES *.cpp)
file(GLOB_RECURSE C_SOURCES *.c)
add_library(BIRDBRAIN_INTERFACE INTERFACE ${CPP_SOURCES} ${C_SOURCES})

# Include directories
glob_directories(BIRDBRAIN_INCLUDE "../core")
target_include_directories(BIRDBRAIN_INTERFACE INTERFACE ${BIRDBRAIN_INCLUDE})

# Requires Libraries
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Links Libraries
if(TARGET SDL2::SDL2main)
    target_link_libraries(BIRDBRAIN_INTERFACE INTERFACE SDL2::SDL2main dl)
endif()
target_link_libraries(BIRDBRAIN_INTERFACE INTERFACE SDL2::SDL2 dl)

message(${CMAKE_CXX_COMPILER_ID})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    target_compile_options(BIRDBRAIN_INTERFACE INTERFACE -Winteger-overflow)
endif()