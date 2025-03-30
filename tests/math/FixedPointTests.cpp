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

    TEST(FixedPoint, exactPositiveDecimalConstructor) {
        EXPECT_EQ((0.9765625_fxd).getBase(), 2000);
        EXPECT_EQ((0.68505859375_fxd).getBase(), 1403);
        EXPECT_EQ((0.46240234375_fxd).getBase(), 947);
        EXPECT_EQ((0.20654296875_fxd).getBase(), 423);
        EXPECT_EQ((0.1142578125_fxd).getBase(), 234);
        EXPECT_EQ((0.00048828125_fxd).getBase(), 1);
        EXPECT_EQ((0.000_fxd).getBase(), 0);

        EXPECT_EQ((0.99609375_fx).getBase(), 255);
        EXPECT_EQ((0.37109375_fx).getBase(), 95);
        EXPECT_EQ((0.1640625_fx).getBase(), 42);
        EXPECT_EQ((0.00390625_fx).getBase(), 1);
        EXPECT_EQ((0.000_fx).getBase(), 0);
    }
    
    TEST(FixedPoint, roundedPositiveDecimalConstructor) {

        EXPECT_EQ((FixedPoint<int64,62>("0.999999999999999999999999")).getBase(), uint64(1) << 62);
        EXPECT_EQ((FixedPoint<int64,61>("0.999999999999999999999999")).getBase(), uint64(1) << 61);
        EXPECT_EQ((FixedPoint<int64,60>("0.999999999999999999999999")).getBase(), uint64(1) << 60);
        EXPECT_EQ((FixedPoint<int64,59>("0.999999999999999999999999")).getBase(), uint64(1) << 59);

        EXPECT_EQ((FixedPoint<int64,63>("0.27238237")).getBase(), 157017745924389449 << 4);
        EXPECT_EQ((FixedPoint<int64,62>("0.27238237")).getBase(), 157017745924389449 << 3);
        EXPECT_EQ((FixedPoint<int64,61>("0.27238237")).getBase(), 157017745924389449 << 2);
        EXPECT_EQ((FixedPoint<int64,60>("0.27238237")).getBase(), 157017745924389449 << 1);
        EXPECT_EQ((FixedPoint<int64,59>("0.27238237")).getBase(), 157017745924389449);

        EXPECT_EQ((0.9999999_fxd).getBase(), 2048);
        EXPECT_EQ((0.9943789_fxd).getBase(), 2036);
        EXPECT_EQ((0.589432_fxd).getBase(), 1207);
        EXPECT_EQ((0.2343598423_fxd).getBase(), 480);
        EXPECT_EQ((0.61398_fxd).getBase(), 1257);
        EXPECT_EQ((0.19424_fxd).getBase(), 398);
        EXPECT_EQ((0.002_fxd).getBase(), 4);
        EXPECT_EQ((0.001_fxd).getBase(), 2);
        
        EXPECT_EQ((0.9999999_fx).getBase(), 256);
        EXPECT_EQ((0.9943789_fx).getBase(), 255);
        EXPECT_EQ((0.589432_fx).getBase(), 151);
        EXPECT_EQ((0.2343598423_fx).getBase(), 60);
        EXPECT_EQ((0.61398_fx).getBase(), 157);
        EXPECT_EQ((0.19424_fx).getBase(), 50);
        EXPECT_EQ((0.002_fx).getBase(), 1);
        EXPECT_EQ((0.001_fx).getBase(), 0);
    }
    
    TEST(FixedPoint, exactNegativeDecimalConstructor) {
        EXPECT_EQ((-0.9765625_fxd).getBase(), -2000);
        EXPECT_EQ((-0.68505859375_fxd).getBase(), -1403);
        EXPECT_EQ((-0.46240234375_fxd).getBase(), -947);
        EXPECT_EQ((-0.20654296875_fxd).getBase(), -423);
        EXPECT_EQ((-0.1142578125_fxd).getBase(), -234);
        EXPECT_EQ((-0.00048828125_fxd).getBase(), -1);
        EXPECT_EQ((-0.000_fxd).getBase(), -0);

        EXPECT_EQ((-0.99609375_fx).getBase(), -255);
        EXPECT_EQ((-0.37109375_fx).getBase(), -95);
        EXPECT_EQ((-0.1640625_fx).getBase(), -42);
        EXPECT_EQ((-0.00390625_fx).getBase(), -1);
        EXPECT_EQ((-0.000_fx).getBase(), -0);
    }
    
    TEST(FixedPoint, roundedNegativeDecimalConstructor) {

        EXPECT_EQ((FixedPoint<int64,62>("-0.999999999999999999999999")).getBase(), int64(-1) << 62);
        EXPECT_EQ((FixedPoint<int64,61>("-0.999999999999999999999999")).getBase(), int64(-1) << 61);
        EXPECT_EQ((FixedPoint<int64,60>("-0.999999999999999999999999")).getBase(), int64(-1) << 60);
        EXPECT_EQ((FixedPoint<int64,59>("-0.999999999999999999999999")).getBase(), int64(-1) << 59);

        EXPECT_EQ((FixedPoint<int64,63>("-0.27238237")).getBase(), -157017745924389449 << 4);
        EXPECT_EQ((FixedPoint<int64,62>("-0.27238237")).getBase(), -157017745924389449 << 3);
        EXPECT_EQ((FixedPoint<int64,61>("-0.27238237")).getBase(), -157017745924389449 << 2);
        EXPECT_EQ((FixedPoint<int64,60>("-0.27238237")).getBase(), -157017745924389449 << 1);
        EXPECT_EQ((FixedPoint<int64,59>("-0.27238237")).getBase(), -157017745924389449);

        EXPECT_EQ((-0.9999999_fxd).getBase(), -2048);
        EXPECT_EQ((-0.9943789_fxd).getBase(), -2036);
        EXPECT_EQ((-0.589432_fxd).getBase(), -1207);
        EXPECT_EQ((-0.2343598423_fxd).getBase(), -480);
        EXPECT_EQ((-0.61398_fxd).getBase(), -1257);
        EXPECT_EQ((-0.19424_fxd).getBase(), -398);
        EXPECT_EQ((-0.002_fxd).getBase(), -4);
        EXPECT_EQ((-0.001_fxd).getBase(), -2);
        
        EXPECT_EQ((-0.9999999_fx).getBase(), -256);
        EXPECT_EQ((-0.9943789_fx).getBase(), -255);
        EXPECT_EQ((-0.589432_fx).getBase(), -151);
        EXPECT_EQ((-0.2343598423_fx).getBase(), -60);
        EXPECT_EQ((-0.61398_fx).getBase(), -157);
        EXPECT_EQ((-0.19424_fx).getBase(), -50);
        EXPECT_EQ((-0.002_fx).getBase(), -1);
        EXPECT_EQ((-0.001_fx).getBase(), -0);
    }
    
    TEST(FixedPoint, mixedFixedPointConstructor) {
        EXPECT_EQ((2342.24782_fx).getBase(), 599615);
        EXPECT_EQ((1231.35498_fx).getBase(), 315227);
        EXPECT_EQ((-234.35559_fx).getBase(), -59995);

        EXPECT_EQ((23238421.213412_fxd).getBase(), 47592286645);
        EXPECT_EQ((8.23192381_fxd).getBase(), 16859);
        EXPECT_EQ((4503599627370495.99951171875_fxd).getBase(), 9223372036854775807);
    }
    
    TEST(FixedPoint, reachAssertFail) {
        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(4503599627370495.999999999999_fxd, ".*");
        EXPECT_DEATH(9000000000000000_fxd, ".*");
        EXPECT_DEATH(19000000023423400000_fxd, ".*");
        EXPECT_DEATH(8388607.99999999_fx, ".*");
        EXPECT_DEATH((FixedPoint<int64,63>("-1.999999999999999999999999")), ".*");
        EXPECT_DEATH((FixedPoint<int64,63>("0.999999999999999999999999")), ".*");
        #endif
        EXPECT_TRUE(true);
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