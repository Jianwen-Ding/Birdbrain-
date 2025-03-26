#include <gtest/gtest.h>

#include "DetMathInt.hpp"

TEST(DeterministicMathFunctions, Sqrt) {
  EXPECT_EQ(DetMathInt::sqrt(9), 3);
  EXPECT_EQ(DetMathInt::sqrt(12), 3);
  EXPECT_EQ(DetMathInt::sqrt(0), 0);
  EXPECT_EQ(DetMathInt::sqrt(3), 1);
  EXPECT_EQ(DetMathInt::sqrt(8), 2);
  EXPECT_EQ(DetMathInt::sqrt(1765), 42);
  EXPECT_EQ(DetMathInt::sqrt(34789), 186);
  EXPECT_EQ(DetMathInt::sqrt(2294967295), 47905);
  EXPECT_EQ(DetMathInt::sqrt(904534), 951);
  EXPECT_EQ(DetMathInt::sqrt(4294967295), 65535);
  EXPECT_EQ(DetMathInt::sqrt(422309423904u), 649853);
  EXPECT_EQ(DetMathInt::sqrt(4534345343701615u), 67337547u);
  EXPECT_EQ(DetMathInt::sqrt(18446744073709551615u), 4294967295u);
}

TEST(DeterministicMathFunctions, Log2) {
  EXPECT_EQ(DetMathInt::log2(3), 1);
  EXPECT_EQ(DetMathInt::log2(8), 3);
  EXPECT_EQ(DetMathInt::log2(92), 6);
  EXPECT_EQ(DetMathInt::log2(12), 3);
  EXPECT_EQ(DetMathInt::log2(1765), 10);
  EXPECT_EQ(DetMathInt::log2(34789), 15);
  EXPECT_EQ(DetMathInt::log2(904534), 19);
  EXPECT_EQ(DetMathInt::log2(1723365), 20);
  EXPECT_EQ(DetMathInt::log2(2294967295), 31);
  EXPECT_EQ(DetMathInt::log2(4294967295), 31);
  EXPECT_EQ(DetMathInt::log2(422309423904u), 38);
  EXPECT_EQ(DetMathInt::log2(93343452394992u), 46);
  EXPECT_EQ(DetMathInt::log2(4534345343701615u), 52);
  EXPECT_EQ(DetMathInt::log2(892534345343701615u), 59);
  EXPECT_EQ(DetMathInt::log2(18446744073709551615u), 63);
}
// Tests rounded integer sqrt method

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}