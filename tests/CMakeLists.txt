# Testing cmake, made so that it could be easily excluded in build case

include(FetchContent)

FetchContent_Declare(
  google_test
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.0
)
FetchContent_MakeAvailable(google_test)

include(GoogleTest)

add_library(TEST_INTERFACE INTERFACE)
target_link_libraries(
  TEST_INTERFACE INTERFACE
  GTest::gtest_main
  BIRDBRAIN_INTERFACE
)

function (gtest_insert test_name test_exec)
  set(FIN_NAME "gtest_${test_name}")

  combine_target(${test_name} ${test_exec} "TEST_INTERFACE")

  gtest_discover_tests(${test_name}
    TEST_PREFIX "${FIN_NAME}_"
    PROPERTIES TIMEOUT 120
  )
endfunction()

file(GLOB_RECURSE TEST_CPP_SOURCES *.cpp)
file(GLOB_RECURSE TEST_C_SOURCES *.c)
set(POSSIBLE_EXECUTABLES ${TEST_CPP_SOURCES} ${TEST_C_SOURCES})

foreach(TEST_EXEC ${POSSIBLE_EXECUTABLES})
  set(TEST_HELPER ${TEST_EXEC})
  get_filename_component(FILENAME_WITH_EXT "${TEST_HELPER}" NAME)
  string(REGEX REPLACE "\\.(cpp|c)$" "" BASE_FILENAME "${FILENAME_WITH_EXT}")

  gtest_insert(${BASE_FILENAME} ${TEST_EXEC})
endforeach()

