#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor

    // Due to char strings being one of the more complex constructors an additional region is warranted
    #pragma region Char String Constructor
    TEST(FixedPoint, positiveIntegerConstructor) {
        EXPECT_EQ((0_fx).getBase(), 0);
        EXPECT_EQ((0_fxd).getBase(), 0);
        EXPECT_EQ((312312_fx).getBase(), (312312 << 8));
        EXPECT_EQ((2122_fx).getBase(), (2122 << 8));
        EXPECT_EQ((22_fx).getBase(), (22 << 8));
        EXPECT_EQ((2342232_fx).getBase(), (2342232 << 8));
        EXPECT_EQ((8388607_fx).getBase(), (8388607<< 8));
        EXPECT_EQ((312312_fxd).getBase(), (uint64(312312) << 11));
        EXPECT_EQ((2122_fxd).getBase(), (uint64(2122) << 11));
        EXPECT_EQ((22_fxd).getBase(), (uint64(22) << 11));
        EXPECT_EQ((2342232_fxd).getBase(), (uint64(2342232) << 11));
        EXPECT_EQ((8388607_fxd).getBase(), (uint64(8388607)<< 11));
        EXPECT_EQ((8388607_fxd).getBase(), (uint64(8388607)<< 11));
        EXPECT_EQ((934934993_fxd).getBase(), (uint64(934934993u)<< 11));
        EXPECT_EQ((13976329829_fxd).getBase(), (uint64(13976329829u)<< 11));
        EXPECT_EQ((23233248923982_fxd).getBase(), (uint64(23233248923982u)<< 11));
        EXPECT_EQ((4503599627370495_fxd).getBase(), (uint64(4503599627370495u)<< 11));
        EXPECT_EQ((FixedPoint<int32, 5>("53392309")).getBase(), uint64(53392309u) << 5);
        EXPECT_EQ((FixedPoint<int8, 3>("5")).getBase(), uint64(5u) << 3);
        EXPECT_EQ((FixedPoint<int64, 0>("9223372036854775807")).getBase(), uint64(9223372036854775807u));
    }

    TEST(FixedPoint, negativeIntegerConstructor) {
        EXPECT_EQ((-312312_fx).getBase(), -(312312 << 8));
        EXPECT_EQ((-2122_fx).getBase(), -(2122 << 8));
        EXPECT_EQ((-22_fx).getBase(), -(22 << 8));
        EXPECT_EQ((-2342232_fx).getBase(), -(2342232 << 8));
        EXPECT_EQ((-8388607_fx).getBase(), -(8388607<< 8));
        EXPECT_EQ((-312312_fxd).getBase(), -(int64(312312) << 11));
        EXPECT_EQ((-2122_fxd).getBase(), -(int64(2122) << 11));
        EXPECT_EQ((-22_fxd).getBase(), -(int64(22) << 11));
        EXPECT_EQ((-2342232_fxd).getBase(), -(int64(2342232) << 11));
        EXPECT_EQ((-8388607_fxd).getBase(), -(int64(8388607)<< 11));
        EXPECT_EQ((-8388607_fxd).getBase(), -(int64(8388607)<< 11));
        EXPECT_EQ((-934934993_fxd).getBase(), -(int64(934934993u)<< 11));
        EXPECT_EQ((-13976329829_fxd).getBase(), -(int64(13976329829u)<< 11));
        EXPECT_EQ((-23233248923982_fxd).getBase(), -(int64(23233248923982u)<< 11));
        EXPECT_EQ((-4503599627370495_fxd).getBase(), -(int64(4503599627370495u)<< 11));
        EXPECT_EQ((FixedPoint<int32, 5>("-53392309")).getBase(), -(int64(53392309u) << 5));
        EXPECT_EQ((FixedPoint<int8, 3>("-5")).getBase(), -(int64(5u) << 3));
        EXPECT_EQ((FixedPoint<int64, 0>("-9223372036854775807")).getBase(), -int64(9223372036854775807u));
    }

    TEST(FixedPoint, purePositiveDecimalConstructor) {
        EXPECT_EQ((0.00390625_fx).getBase(), 1);
        EXPECT_EQ((0.002_fx).getBase(), 1);
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