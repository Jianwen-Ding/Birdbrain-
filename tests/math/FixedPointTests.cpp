#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor

    // Due to char strings being one of the more complex constructors an additional region is warranted
    #pragma region Char String Constructor
    TEST(FixedPoint, pureIntegerConstructor) {
        EXPECT_EQ((312312_fx).getBase(), (312312 << 8));
        EXPECT_EQ((2122_fx).getBase(), (2122 << 8));
        EXPECT_EQ((22_fx).getBase(), (22 << 8));
        EXPECT_EQ((2342232_fx).getBase(), (2342232 << 8));
        EXPECT_EQ((8388607_fx).getBase(), (8388607<< 8));
    }
    #pragma endregion
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