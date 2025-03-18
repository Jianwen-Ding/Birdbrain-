#include <gtest/gtest.h>

#include "DetMath.hpp"

// Tests integer sqrt method
TEST(DeterministicMathFunctions, Sqrt) {
  EXPECT_EQ(DetMath::sqrt(9), 3);
  EXPECT_EQ(DetMath::sqrt(12), 3);
  EXPECT_EQ(DetMath::sqrt(0), 0);
  EXPECT_EQ(DetMath::sqrt(3), 1);
  EXPECT_EQ(DetMath::sqrt(8), 2);
  EXPECT_EQ(DetMath::sqrt(1765), 42);
  EXPECT_EQ(DetMath::sqrt(34789), 186);
  EXPECT_EQ(DetMath::sqrt(2294967295), 47905);
  EXPECT_EQ(DetMath::sqrt(904534), 951);
  EXPECT_EQ(DetMath::sqrt(4294967295), 65535);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}