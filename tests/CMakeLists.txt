#Testing cmake, included so that it could be easily excluded in build case

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_executable(hello_test hello_test.cpp)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

add_test(hello_gtest hello_test)