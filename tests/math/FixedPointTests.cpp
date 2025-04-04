#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor

    // Due to char strings being one of the more complex constructors an additional region is warranted
    #pragma region Char String Constructor

    // Negative number use constructor instead of literal due to literals ignoring negative values and just constructing it then applying -


    TEST(FixedPoint, positiveIntegerConstructor) {
        EXPECT_EQ((0_fx).getBase(), 0);
        EXPECT_EQ((0_fx).getBase(), 0);
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
        EXPECT_EQ((fixed("-312312")).getBase(), -(312312 << 8));
        EXPECT_EQ((fixed("-2122")).getBase(), -(2122 << 8));
        EXPECT_EQ((fixed("-22")).getBase(), -(22 << 8));
        EXPECT_EQ((fixed("-2342232")).getBase(), -(2342232 << 8));
        EXPECT_EQ((fixed("-8388607")).getBase(), -(8388607<< 8));
        EXPECT_EQ((doubleFixed("-312312")).getBase(), -(int64(312312) << 11));
        EXPECT_EQ((doubleFixed("-2122")).getBase(), -(int64(2122) << 11));
        EXPECT_EQ((doubleFixed("-22")).getBase(), -(int64(22) << 11));
        EXPECT_EQ((doubleFixed("-2342232")).getBase(), -(int64(2342232) << 11));
        EXPECT_EQ((doubleFixed("-8388607")).getBase(), -(int64(8388607)<< 11));
        EXPECT_EQ((doubleFixed("-8388607")).getBase(), -(int64(8388607)<< 11));
        EXPECT_EQ((doubleFixed("-934934993")).getBase(), -(int64(934934993u)<< 11));
        EXPECT_EQ((doubleFixed("-13976329829")).getBase(), -(int64(13976329829u)<< 11));
        EXPECT_EQ((doubleFixed("-23233248923982")).getBase(), -(int64(23233248923982u)<< 11));
        EXPECT_EQ((doubleFixed("-4503599627370495")).getBase(), -(int64(4503599627370495u)<< 11));
        EXPECT_EQ((FixedPoint<int32, 5>("-53392309")).getBase(), -(int64(53392309u) << 5));
        EXPECT_EQ((FixedPoint<int8, 3>("-5")).getBase(), -(int64(5u) << 3));
        EXPECT_EQ((FixedPoint<int64, 0>("-9223372036854775807")).getBase(), -int64(9223372036854775807u));
        EXPECT_EQ((FixedPoint<int64, 0>("-9223372036854775808")).getBase(), -int64(9223372036854775808u));
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
        EXPECT_EQ((doubleFixed("-0.9765625")).getBase(), -2000);
        EXPECT_EQ((doubleFixed("-0.68505859375")).getBase(), -1403);
        EXPECT_EQ((doubleFixed("-0.46240234375")).getBase(), -947);
        EXPECT_EQ((doubleFixed("-0.20654296875")).getBase(), -423);
        EXPECT_EQ((doubleFixed("-0.1142578125")).getBase(), -234);
        EXPECT_EQ((doubleFixed("-0.00048828125")).getBase(), -1);
        EXPECT_EQ((doubleFixed("-0.000")).getBase(), -0);

        EXPECT_EQ((fixed("-0.99609375")).getBase(), -255);
        EXPECT_EQ((fixed("-0.37109375")).getBase(), -95);
        EXPECT_EQ((fixed("-0.1640625")).getBase(), -42);
        EXPECT_EQ((fixed("-0.00390625")).getBase(), -1);
        EXPECT_EQ((fixed("-0.000")).getBase(), -0);
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

        EXPECT_EQ((doubleFixed("-0.9999999")).getBase(), -2048);
        EXPECT_EQ((doubleFixed("-0.9943789")).getBase(), -2036);
        EXPECT_EQ((doubleFixed("-0.589432")).getBase(), -1207);
        EXPECT_EQ((doubleFixed("-0.2343598423")).getBase(), -480);
        EXPECT_EQ((doubleFixed("-0.61398")).getBase(), -1257);
        EXPECT_EQ((doubleFixed("-0.19424")).getBase(), -398);
        EXPECT_EQ((doubleFixed("-0.002")).getBase(), -4);
        EXPECT_EQ((doubleFixed("-0.001")).getBase(), -2);
        
        EXPECT_EQ((fixed("-0.9999999")).getBase(), -256);
        EXPECT_EQ((fixed("-0.9943789")).getBase(), -255);
        EXPECT_EQ((fixed("-0.589432")).getBase(), -151);
        EXPECT_EQ((fixed("-0.2343598423")).getBase(), -60);
        EXPECT_EQ((fixed("-0.61398")).getBase(), -157);
        EXPECT_EQ((fixed("-0.19424")).getBase(), -50);
        EXPECT_EQ((fixed("-0.002")).getBase(), -1);
        EXPECT_EQ((fixed("-0.001")).getBase(), -0);
    }
    
    TEST(FixedPoint, mixedFixedPointConstructor) {
        EXPECT_EQ((2342.24782_fx).getBase(), 599615);
        EXPECT_EQ((1231.35498_fx).getBase(), 315227);
        EXPECT_EQ((-234.35559_fx).getBase(), -59995);

        EXPECT_EQ((23238421.213412_fxd).getBase(), 47592286645);
        EXPECT_EQ((8.23192381_fxd).getBase(), 16859);
        EXPECT_EQ((4503599627370495.99951171875_fxd).getBase(), 9223372036854775807);
        EXPECT_EQ((doubleFixed("-4503599627370496.000000")).getBase(), -9223372036854775807 - 1);
        EXPECT_EQ((fixed("-8388608.000000")).getBase(), -2147483648);

        EXPECT_EQ((FixedPoint<int16,5>("3.234198")).getBase(), 103);
        EXPECT_EQ((FixedPoint<int8,7>("0.848352")).getBase(), 109);
    }
    
    TEST(FixedPoint, reachAssertFail) {
        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(doubleFixed("-4503599627370496.000250"),".*");
        EXPECT_DEATH(fixed("-8388608.004250"),".*");
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

// Tests for casting to different numeric types
#pragma region Casting

    TEST(FixedPoint, floatingPointCast) {
        EXPECT_EQ((float)0_fx, 0.f);
        EXPECT_EQ((float)2.6875_fx, 2.6875f);
        EXPECT_EQ((float)473.1875_fx, 473.1875f);
        EXPECT_EQ((float)-2.6875_fx, -2.6875f);
        EXPECT_EQ((float)-473.1875_fx, -473.1875f);
        EXPECT_EQ((float)-121138.1875_fx, -121138.1875f);
        EXPECT_EQ((float)121138.1875_fx, 121138.1875f);
        EXPECT_EQ((float)-0.71875_fx, -0.71875f);
        EXPECT_EQ((float)0.71875_fx, 0.71875f);
        EXPECT_EQ((float)0.71875_fx, 0.71875f);
        EXPECT_EQ((float)8388607_fx, 8388607.0f);
        EXPECT_EQ((float)0.71875_fx, 0.71875f);

        EXPECT_EQ((float)0_fxd, 0);
        EXPECT_EQ((float)2.6875_fxd, 2.6875f);
        EXPECT_EQ((float)473.1875_fxd, 473.1875f);
        EXPECT_EQ((float)-2.6875_fxd, -2.6875f);
        EXPECT_EQ((float)-473.1875_fxd, -473.1875f);
        EXPECT_EQ((float)-121138.1875_fxd, -121138.1875f);
        EXPECT_EQ((float)121138.1875_fxd, 121138.1875f);
        EXPECT_EQ((float)-0.71875_fxd, -0.71875f);
        EXPECT_EQ((float)0.71875_fxd, 0.71875f);
        EXPECT_EQ((float)4503599627370495_fxd, 4503599358935040.0f);
        EXPECT_EQ((float)doubleFixed("-4503599627370496"), -4503599627370496.0f);

        EXPECT_EQ((float)(FixedPoint<int16,5>("3.234198")), 3.21875f);
        EXPECT_EQ((float)(FixedPoint<int8,7>("0.848352")), 0.8515625f);

        EXPECT_EQ((float)(FixedPoint<int64,5>("3.234198")), 3.21875f);
        EXPECT_EQ((float)(FixedPoint<int64,7>("0.848352")), 0.8515625f);

        EXPECT_EQ((float)(FixedPoint<int64,59>("0.0009720075980754923350435348083919961936771869659423828125")), 0.0009720075759105384349822998046875f);
        EXPECT_EQ((float)(FixedPoint<int64,39>("1019.22383915960745071060955524444580078125")), 1019.22381591796875f);
    }

    TEST(FixedPoint, doubleFloatingPointCast) {
        EXPECT_EQ((double)0_fx, 0);
        EXPECT_EQ((double)2.6875_fx, 2.6875);
        EXPECT_EQ((double)473.1875_fx, 473.1875);
        EXPECT_EQ((double)-2.6875_fx, -2.6875);
        EXPECT_EQ((double)-473.1875_fx, -473.1875);
        EXPECT_EQ((double)-121138.1875_fx, -121138.1875);
        EXPECT_EQ((double)121138.1875_fx, 121138.1875);
        EXPECT_EQ((double)-0.71875_fx, -0.71875);
        EXPECT_EQ((double)0.71875_fx, 0.71875);
        EXPECT_EQ((double)0.71875_fx, 0.71875);
        EXPECT_EQ((double)182.023112_fx, 182.0234375);
        EXPECT_EQ((double)8388607_fx, 8388607.0);

        EXPECT_EQ((double)0_fxd, 0);
        EXPECT_EQ((double)0.11212121_fxd, 0.1123046875);
        EXPECT_EQ((double)2.6875_fxd, 2.6875);
        EXPECT_EQ((double)473.1875_fxd, 473.1875);
        EXPECT_EQ((double)-2.6875_fxd, -2.6875);
        EXPECT_EQ((double)-473.1875_fxd, -473.1875);
        EXPECT_EQ((double)-121138.1875_fxd, -121138.1875);
        EXPECT_EQ((double)121138.1875_fxd, 121138.1875);
        EXPECT_EQ((double)-0.71875_fxd, -0.71875);
        EXPECT_EQ((double)0.71875_fxd, 0.71875);
        EXPECT_EQ((double)4503599627370494_fxd, 4503599627370494.0);
        EXPECT_EQ((double)doubleFixed("-4503599627370496"), -4503599627370496.0);

        EXPECT_EQ((double)(FixedPoint<int16,5>("3.234198")), 3.21875);
        EXPECT_EQ((double)(FixedPoint<int8,7>("0.848352")), 0.8515625);

        EXPECT_EQ((double)(FixedPoint<int64,5>("3.234198")), 3.21875);
        EXPECT_EQ((double)(FixedPoint<int64,7>("0.848352")), 0.8515625);

        EXPECT_EQ((double)(FixedPoint<int64,59>("0.0009720075980754923350435348083919961936771869659423828125")), 0.0009720075980754923350435348083919961936771869659423828125);
        EXPECT_EQ((double)(FixedPoint<int64,39>("1019.22383915960745071060955524444580078125")), 1019.22383915960745071060955524444580078125);
    }
#pragma endregion

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}