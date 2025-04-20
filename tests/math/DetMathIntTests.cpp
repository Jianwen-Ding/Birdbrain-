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

TEST(DeterministicMathFunctions, SqrtConstexpr) {
  constexpr int test1 = DetMathInt::sqrt(9);
  EXPECT_EQ(test1, 3);
  
  constexpr int test2 = DetMathInt::sqrt(12);
  EXPECT_EQ(test2, 3);
  
  constexpr int test3 = DetMathInt::sqrt(0);
  EXPECT_EQ(test3, 0);
  
  constexpr int test4 = DetMathInt::sqrt(3);
  EXPECT_EQ(test4, 1);
  
  constexpr int test5 = DetMathInt::sqrt(8);
  EXPECT_EQ(test5, 2);
  
  constexpr int test6 = DetMathInt::sqrt(1765);
  EXPECT_EQ(test6, 42);
  
  constexpr int test7 = DetMathInt::sqrt(34789);
  EXPECT_EQ(test7, 186);
  
  constexpr int test8 = DetMathInt::sqrt(2294967295);
  EXPECT_EQ(test8, 47905);
  
  constexpr int test9 = DetMathInt::sqrt(904534);
  EXPECT_EQ(test9, 951);
  
  constexpr int test10 = DetMathInt::sqrt(4294967295);
  EXPECT_EQ(test10, 65535);
  
  constexpr int test11 = DetMathInt::sqrt(422309423904u);
  EXPECT_EQ(test11, 649853);
  
  constexpr int test12 = DetMathInt::sqrt(4534345343701615u);
  EXPECT_EQ(test12, 67337547u);
  
  constexpr int test13 = DetMathInt::sqrt(18446744073709551615u);
  EXPECT_EQ(test13, 4294967295u);
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

TEST(DeterministicMathFunctions, Log2Constexpr) {
  
  constexpr int test1 = DetMathInt::log(16, 2);
  EXPECT_EQ(test1, 4);

  
  constexpr int test2 = DetMathInt::log(81, 3);
  EXPECT_EQ(test2, 4);

  
  constexpr int test3 = DetMathInt::log(100, 10);
  EXPECT_EQ(test3, 2);

  
  constexpr int test4 = DetMathInt::log(1024, 2);
  EXPECT_EQ(test4, 10);

  
  constexpr int test5 = DetMathInt::log(1, 2);
  EXPECT_EQ(test5, 0);

  
  constexpr int test6 = DetMathInt::log(2, 2);
  EXPECT_EQ(test6, 1);

  
  constexpr int test7 = DetMathInt::log(8,2);
  EXPECT_EQ(test7, 3);

  
  constexpr int test8 = DetMathInt::log(27,3);
  EXPECT_EQ(test8, 3);

  
  constexpr int test9 = DetMathInt::log(10, 1);
  EXPECT_EQ(test9, 0);

  constexpr uint32 test10 = DetMathInt::log(std::numeric_limits<uint32>::max(), 2);
  EXPECT_EQ(test10, 31);

  constexpr uint64 test11 = DetMathInt::log(std::numeric_limits<uint64>::max(), 2);
  EXPECT_EQ(test11, 63);

  constexpr uint32 test12 = DetMathInt::log(std::numeric_limits<uint32>::max(), 10);
  EXPECT_EQ(test12, 9);

  constexpr uint64 test13 = DetMathInt::log(std::numeric_limits<uint64>::max(), 10);
  EXPECT_EQ(test13, 19);
  
  constexpr int test14 = DetMathInt::log(2,3);
  EXPECT_EQ(test14, 0);

  
  constexpr int test15 = DetMathInt::log(5,10);
  EXPECT_EQ(test15, 0);

  
  constexpr int test16 = DetMathInt::log(8,16);
  EXPECT_EQ(test16, 0);

  
  constexpr int test17 = DetMathInt::log(16u,2);
  EXPECT_EQ(test17, 4);

  
  constexpr int test18 = DetMathInt::log(16ul,2);
  EXPECT_EQ(test18, 4);

  
  constexpr int test19 = DetMathInt::log(16ull,2);
  EXPECT_EQ(test19, 4);

  
  constexpr int test20 = DetMathInt::log(16,2);
  EXPECT_EQ(test20, 4);
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

TEST(DeterministicMathFunctions, LogConstexpr) {
  constexpr int test1 = DetMathInt::log(16, 2);
  EXPECT_EQ(test1, 4);

  constexpr int test2 = DetMathInt::log(81, 3);
  EXPECT_EQ(test2, 4);

  constexpr int test3 = DetMathInt::log(100, 10);
  EXPECT_EQ(test3, 2);

  constexpr int test4 = DetMathInt::log(1024, 2);
  EXPECT_EQ(test4, 10);

  constexpr int test5 = DetMathInt::log(1, 2);
  EXPECT_EQ(test5, 0);

  constexpr int test6 = DetMathInt::log(2, 2);
  EXPECT_EQ(test6, 1);

  constexpr int test7 = DetMathInt::log(8,2);
  EXPECT_EQ(test7, 3);

  constexpr int test8 = DetMathInt::log(27,3);
  EXPECT_EQ(test8, 3);

  constexpr int test9 = DetMathInt::log(10, 1);
  EXPECT_EQ(test9, 0);

  constexpr uint32 test10 = DetMathInt::log(std::numeric_limits<uint32>::max(), 2);
  EXPECT_EQ(test10, 31);

  constexpr uint64 test11 = DetMathInt::log(std::numeric_limits<uint64>::max(), 2);
  EXPECT_EQ(test11, 63);

  constexpr uint64 test12 = DetMathInt::log(std::numeric_limits<uint32>::max(), 10);
  EXPECT_EQ(test12, 9);

  constexpr uint64 test13 = DetMathInt::log(std::numeric_limits<uint32>::max(), 10);
  EXPECT_EQ(test13, 9);

  constexpr int test14 = DetMathInt::log(2,3);
  EXPECT_EQ(test14, 0);

  constexpr int test15 = DetMathInt::log(5,10);
  EXPECT_EQ(test15, 0);

  constexpr int test16 = DetMathInt::log(8,16);
  EXPECT_EQ(test16, 0);

  constexpr int test17 = DetMathInt::log(16u,2);
  EXPECT_EQ(test17, 4);

  constexpr int test18 = DetMathInt::log(16ul,2);
  EXPECT_EQ(test18, 4);

  constexpr int test19 = DetMathInt::log(16ull,2);
  EXPECT_EQ(test19, 4);

  constexpr int test20 = DetMathInt::log(16,2);
  EXPECT_EQ(test20, 4);

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

TEST(DeterministicMathFunction, PowConstexpr) {
  constexpr int64 test = DetMathInt::pow(5, (uint8)0);
  EXPECT_EQ(test, 1LL);
  
  constexpr int64 test2 = DetMathInt::pow(-5, (uint8)0);
  EXPECT_EQ(test2, 1LL);

  constexpr int64 test3 = DetMathInt::pow(0, (uint8)0);
  EXPECT_EQ(test3, 1LL);

  constexpr int64 test4 = DetMathInt::pow<int32>(std::numeric_limits<int32>::max(), (uint8)1);
  EXPECT_EQ(test4, (int64)std::numeric_limits<int>::max());

  constexpr int64 test5 = DetMathInt::pow(std::numeric_limits<int32>::min(), (uint8)1);
  EXPECT_EQ(test5, (int64)std::numeric_limits<int>::min());

  constexpr int64 test6 = DetMathInt::pow<int8>(-10, 1);
  EXPECT_EQ(test6, -10LL);

  constexpr int64 test7 = DetMathInt::pow(2, (uint8)3);
  EXPECT_EQ(test7, 8LL);
  
  constexpr int64 test8 = DetMathInt::pow(3, (uint8)4);
  EXPECT_EQ(test8, 81LL);
  
  constexpr int64 test9 = DetMathInt::pow(10, (uint8)5);
  EXPECT_EQ(test9, 100000LL);

  constexpr int64 test10 = DetMathInt::pow<int32>(2, 10);
  EXPECT_EQ(test10, 1024LL);

  constexpr int64 test11 = DetMathInt::pow<int64>(2, 62);
  EXPECT_EQ(test11, 4611686018427387904LL);

  constexpr int64 test12 = DetMathInt::pow(-2, (uint8)3);
  EXPECT_EQ(test12, (int64)-8);

  constexpr int64 test13 = DetMathInt::pow(-3, (uint8)4);
  EXPECT_EQ(test13, 81LL);

  constexpr int64 test14 = DetMathInt::pow<int8>(-3, 3);
  EXPECT_EQ(test14, (int64)-27);

  constexpr int64 test15 = DetMathInt::pow<uint64>(-2, 62);
  EXPECT_EQ(test15, 4611686018427387904LL);

  constexpr int64 test16 = DetMathInt::pow(0, (uint8)1);
  EXPECT_EQ(test16, 0LL);

  constexpr int64 test17 = DetMathInt::pow(1, (uint8)255);
  EXPECT_EQ(test17, 1LL);

  constexpr uint64 test19 = DetMathInt::pow<uint32>(12345u, 0);
  EXPECT_EQ(test, 1ULL);

  constexpr uint64 test20 = DetMathInt::pow(std::numeric_limits<uint32>::max(), (uint8)0);
  EXPECT_EQ(test20, 1ULL);

  constexpr uint64 test21 = DetMathInt::pow<uint8>(10u, 0);
  EXPECT_EQ(test21, 1ULL);

  constexpr uint64 test22 = DetMathInt::pow(98765ull, (uint8)1);
  EXPECT_EQ(test22, 98765ULL);

  EXPECT_EQ(DetMathInt::pow(std::numeric_limits<uint32>::max(), (uint8)1), (uint64)std::numeric_limits<uint32>::max());

  constexpr uint64 test23 = DetMathInt::pow<uint8>(10u, 1);
  EXPECT_EQ(test23, 10ULL);

  constexpr uint64 test24 = DetMathInt::pow(10u, (uint8)5);
  EXPECT_EQ(test24, 100000ULL);

  constexpr uint64 test25 = DetMathInt::pow<uint>(2u, 10);
  EXPECT_EQ(test25, 1024ULL);

  constexpr uint64 test26 = DetMathInt::pow<uint64>(2ull, 63);
  EXPECT_EQ(test26, 9223372036854775808ULL);

  constexpr uint64 test27 = DetMathInt::pow(0u, (uint8)1);
  EXPECT_EQ(test27, 0ULL);

  constexpr uint64 test28 = DetMathInt::pow(0u, (uint8)5);
  EXPECT_EQ(test28, 0ULL);

  constexpr uint64 test29 = DetMathInt::pow(0u, (uint8)255);
  EXPECT_EQ(test29, 0ULL);

  constexpr uint64 test30 = DetMathInt::pow(1u, (uint8)255);
  EXPECT_EQ(test30, 1ULL);

}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}