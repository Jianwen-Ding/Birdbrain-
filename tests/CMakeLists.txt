#Testing cmake, included so that it could be easily excluded in build case

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_CPP_SOURCES ./tests/src/*.cpp)
file(GLOB_RECURSE TEST_C_SOURCES ./tests/src/*.c)

function (test_insert test_name test_exec)
  set(FIN_NAME "gtest_${test_name}")
  add_executable(${test_name} ${test_exec} ${CPP_SOURCES} ${C_SOURCES} ${TEST_CPP_SOURCES} ${TEST_C_SOURCES})

  target_link_libraries(
    ${test_name} PUBLIC
    GTest::gtest_main
  )
  if(TARGET SDL2::SDL2main)
      target_link_libraries(${test_name} PRIVATE SDL2::SDL2main dl)
  endif()
  target_link_libraries(${test_name} PRIVATE SDL2::SDL2 dl)

  target_include_directories(${test_name} PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/engine/include/rendering
    ${CMAKE_SOURCE_DIR}/engine/include/rendering/SDL
    ${CMAKE_SOURCE_DIR}/engine/include/input
    ${CMAKE_SOURCE_DIR}/engine/include/math
    ${CMAKE_SOURCE_DIR}/engine/include/ecs
    ${CMAKE_SOURCE_DIR}/engine/include/ecs/components
    ${CMAKE_SOURCE_DIR}/tests/include
    ${CMAKE_SOURCE_DIR}/tests/include/rendering
    ${CMAKE_SOURCE_DIR}/tests/include/rendering/SDL
    ${CMAKE_SOURCE_DIR}/tests/include/input
    ${CMAKE_SOURCE_DIR}/tests/include/math
    ${CMAKE_SOURCE_DIR}/tests/include/ecs
    ${CMAKE_SOURCE_DIR}/tests/include/ecs/components
  )

  add_test(${FIN_NAME} ${test_name})
endfunction()

test_insert(main_test main_test.cpp)
test_insert(DetMathIntTests math/DetMathIntTests.cpp)
test_insert(Vector2iTests math/Vector2iTests.cpp)
test_insert(FixedPointTests math/FixedPointTests.cpp)

#test_insert(DetMathTest src/math/DetMathTests.cpp)
#main_gtest 
# add_executable(main_test main_test.cpp)

# target_link_libraries(
#   main_test
#   GTest::gtest_main
# )

# add_test(main_gtest main_test)

