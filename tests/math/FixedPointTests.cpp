#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor
#pragma endregion

// Tests if simple operations between Vectors works
#pragma region Math Operations
#pragma endregion

// Tests if utility functions of Vector2i work as intended 
#pragma region Helper
TEST(Vector2Integer, Magnitude) {
    EXPECT_EQ(FLOAT_IEEE754_REP, true);
}
#pragma endregion

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}