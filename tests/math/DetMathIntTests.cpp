#include <gtest/gtest.h>

#include "DetMathInt.hpp"

// Tests non rounded integer sqrt method
TEST(DeterministicMathFunctions, Sqrt) {
  EXPECT_EQ(DetMathIntN::sqrt(9), 3);
  EXPECT_EQ(DetMathIntN::sqrt(12), 3);
  EXPECT_EQ(DetMathIntN::sqrt(0), 0);
  EXPECT_EQ(DetMathIntN::sqrt(3), 1);
  EXPECT_EQ(DetMathIntN::sqrt(8), 2);
  EXPECT_EQ(DetMathIntN::sqrt(1765), 42);
  EXPECT_EQ(DetMathIntN::sqrt(34789), 186);
  EXPECT_EQ(DetMathIntN::sqrt(2294967295), 47905);
  EXPECT_EQ(DetMathIntN::sqrt(904534), 951);
  EXPECT_EQ(DetMathIntN::sqrt(4294967295), 65535);
  EXPECT_EQ(DetMathIntN::sqrt(422309423904u), 649853);
  EXPECT_EQ(DetMathIntN::sqrt(4534345343701615u), 67337547u);
  EXPECT_EQ(DetMathIntN::sqrt(18446744073709551615u), 4294967295u);
}

TEST(DeterministicMathFunctions, RoundedSqrt) {
  EXPECT_EQ(DetMathIntR::sqrt(9), 3);
  EXPECT_EQ(DetMathIntR::sqrt(12), 4);
  EXPECT_EQ(DetMathIntR::sqrt(0), 0);
  EXPECT_EQ(DetMathIntR::sqrt(3), 2);
  EXPECT_EQ(DetMathIntR::sqrt(8), 3);
  EXPECT_EQ(DetMathIntR::sqrt(1765), 42);
  EXPECT_EQ(DetMathIntR::sqrt(34789), 187);
  EXPECT_EQ(DetMathIntR::sqrt(2294967295), 47906);
  EXPECT_EQ(DetMathIntR::sqrt(904534), 951);
  EXPECT_EQ(DetMathIntR::sqrt(4294967295), 65536);
  EXPECT_EQ(DetMathIntR::sqrt(422309423904u), 649853);
  EXPECT_EQ(DetMathIntR::sqrt(4534345343701615u), 67337548u);
  EXPECT_EQ(DetMathIntR::sqrt(18446744073709551615u), 4294967296u);
}

// Tests rounded integer sqrt method

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}