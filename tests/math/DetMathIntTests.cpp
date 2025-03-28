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

TEST(DeterministicMathFunctions, Log) {
  EXPECT_EQ(DetMathInt::log(16, 2), 4);
  EXPECT_EQ(DetMathInt::log(81, 3), 4);
  EXPECT_EQ(DetMathInt::log(100, 10), 2);
  EXPECT_EQ(DetMathInt::log(1024, 2), 10);
  EXPECT_EQ(DetMathInt::log(1, 2), 0);
  EXPECT_EQ(DetMathInt::log(2, 2), 1);
  EXPECT_EQ(DetMathInt::log(8,2),3);
  EXPECT_EQ(DetMathInt::log(27,3),3);
  EXPECT_EQ(DetMathInt::log(0, 2), 0);
  EXPECT_EQ(DetMathInt::log(10, 1), 0);
  EXPECT_EQ(DetMathInt::log(std::numeric_limits<uint32>::max(), 2), 31);
  EXPECT_EQ(DetMathInt::log(std::numeric_limits<uint64>::max(), 2), 63);
  EXPECT_EQ(DetMathInt::log(std::numeric_limits<uint32>::max(), 10), 9);
  EXPECT_EQ(DetMathInt::log(std::numeric_limits<uint64>::max(), 10), 19);
  EXPECT_EQ(DetMathInt::log(2,3),0);
  EXPECT_EQ(DetMathInt::log(5,10),0);
  EXPECT_EQ(DetMathInt::log(8,16),0);
  EXPECT_EQ(DetMathInt::log(16u,2),4);
  EXPECT_EQ(DetMathInt::log(16ul,2),4);
  EXPECT_EQ(DetMathInt::log(16ull,2),4);
  EXPECT_EQ(DetMathInt::log(16,2),4);
}

TEST(DeterministicMathFunction, Pow) {
  EXPECT_EQ(DetMathInt::pow(5, (uint8)0), 1LL);
  EXPECT_EQ(DetMathInt::pow(-5, (uint8)0), 1LL);
  EXPECT_EQ(DetMathInt::pow(0, (uint8)0), 1LL);
  EXPECT_EQ(DetMathInt::pow(std::numeric_limits<int32>::max(), (uint8)1), (int64)std::numeric_limits<int>::max());
  EXPECT_EQ(DetMathInt::pow(std::numeric_limits<int32>::min(), (uint8)1), (int64)std::numeric_limits<int>::min());
  EXPECT_EQ(DetMathInt::pow<int8>(-10, 1), -10LL);
  EXPECT_EQ(DetMathInt::pow(2, (uint8)3), 8LL);
  EXPECT_EQ(DetMathInt::pow(3, (uint8)4), 81LL);
  EXPECT_EQ(DetMathInt::pow(10, (uint8)5), 100000LL);
  EXPECT_EQ(DetMathInt::pow<int32>(2, 10), 1024LL);
  EXPECT_EQ(DetMathInt::pow<int64>(2, 62), 4611686018427387904LL);
  EXPECT_EQ(DetMathInt::pow(-2, (uint8)3), -8LL);
  EXPECT_EQ(DetMathInt::pow(-3, (uint8)4), 81LL);
  EXPECT_EQ(DetMathInt::pow<int8>(-3, 3), -27LL);
  EXPECT_EQ(DetMathInt::pow<uint64>(-2, 62), 4611686018427387904LL);
  EXPECT_EQ(DetMathInt::pow(0, (uint8)1), 0LL);
  EXPECT_EQ(DetMathInt::pow(1, (uint8)255), 1LL);
  EXPECT_EQ(DetMathInt::pow(-1, (uint8)99), -1LL);
  EXPECT_EQ(DetMathInt::pow<uint32>(12345u, 0), 1ULL);
  EXPECT_EQ(DetMathInt::pow(std::numeric_limits<uint32>::max(), (uint8)0), 1ULL);
  EXPECT_EQ(DetMathInt::pow<uint8>(10u, 0), 1ULL);
  EXPECT_EQ(DetMathInt::pow(98765ull, (uint8)1), 98765ULL);
  EXPECT_EQ(DetMathInt::pow(std::numeric_limits<uint32>::max(), (uint8)1), (uint64)std::numeric_limits<uint32>::max());
  EXPECT_EQ(DetMathInt::pow<uint8>(10u, 1), 10ULL);
  EXPECT_EQ(DetMathInt::pow(10u, (uint8)5), 100000ULL);
  EXPECT_EQ(DetMathInt::pow<uint>(2u, 10), 1024ULL);
  EXPECT_EQ(DetMathInt::pow<uint64>(2ull, 63), 9223372036854775808ULL);
  EXPECT_EQ(DetMathInt::pow(0u, (uint8)1), 0ULL);
  EXPECT_EQ(DetMathInt::pow(0u, (uint8)5), 0ULL);
  EXPECT_EQ(DetMathInt::pow(0u, (uint8)255), 0ULL);
  EXPECT_EQ(DetMathInt::pow(1u, (uint8)255), 1ULL);
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}