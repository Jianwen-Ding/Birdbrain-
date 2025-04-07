#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// Tests if constructors lead to proper values
#pragma region Constructor

    // Due to char strings being one of the more complex constructors an additional region is warranted
    #pragma region Char String Constructor
        // Negative number use constructor instead of literal due to literals ignoring negative values and just constructing it then applying -
        TEST(FixedPointConstructor, positiveIntegerConstructor) {
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
            EXPECT_EQ((FixedPoint<int32, 5, true>("53392309")).getBase(), uint64(53392309u) << 5);
            EXPECT_EQ((FixedPoint<int8, 3, true>("5")).getBase(), uint64(5u) << 3);
            EXPECT_EQ((FixedPoint<int64, 0, true>("9223372036854775807")).getBase(), uint64(9223372036854775807u));
            EXPECT_EQ((0_fxr).getBase(), 0);
            EXPECT_EQ((1_fxr).getBase(), (uint64(1) << 31));
        }

        TEST(FixedPointConstructor, negativeIntegerConstructor) {
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
            EXPECT_EQ((FixedPoint<int32, 5, true>("-53392309")).getBase(), -(int64(53392309u) << 5));
            EXPECT_EQ((FixedPoint<int8, 3, true>("-5")).getBase(), -(int64(5u) << 3));
            EXPECT_EQ((FixedPoint<int64, 0, true>("-9223372036854775807")).getBase(), -int64(9223372036854775807u));
            EXPECT_EQ((FixedPoint<int64, 0, true>("-9223372036854775808")).getBase(), std::numeric_limits<int64>::min());
        }

        TEST(FixedPointConstructor, exactPositiveDecimalConstructor) {
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
        
        TEST(FixedPointConstructor, roundedPositiveDecimalConstructor) {

            EXPECT_EQ((FixedPoint<int64, 62, true>("0.999999999999999999999999")).getBase(), uint64(1) << 62);
            EXPECT_EQ((FixedPoint<int64, 61, true>("0.999999999999999999999999")).getBase(), uint64(1) << 61);
            EXPECT_EQ((FixedPoint<int64, 60, true>("0.999999999999999999999999")).getBase(), uint64(1) << 60);
            EXPECT_EQ((FixedPoint<int64, 59, true>("0.999999999999999999999999")).getBase(), uint64(1) << 59);

            EXPECT_EQ((FixedPoint<int64, 63, true>("0.27238237")).getBase(), 157017745924389449 << 4);
            EXPECT_EQ((FixedPoint<int64, 62, true>("0.27238237")).getBase(), 157017745924389449 << 3);
            EXPECT_EQ((FixedPoint<int64, 61, true>("0.27238237")).getBase(), 157017745924389449 << 2);
            EXPECT_EQ((FixedPoint<int64, 60, true>("0.27238237")).getBase(), 157017745924389449 << 1);
            EXPECT_EQ((FixedPoint<int64, 59, true>("0.27238237")).getBase(), 157017745924389449);

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
        
        TEST(FixedPointConstructor, exactNegativeDecimalConstructor) {
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
        
        TEST(FixedPointConstructor, roundedNegativeDecimalConstructor) {

            EXPECT_EQ((FixedPoint<int64,62, true>("-0.999999999999999999999999")).getBase(), int64(-1) << 62);
            EXPECT_EQ((FixedPoint<int64,61, true>("-0.999999999999999999999999")).getBase(), int64(-1) << 61);
            EXPECT_EQ((FixedPoint<int64,60, true>("-0.999999999999999999999999")).getBase(), int64(-1) << 60);
            EXPECT_EQ((FixedPoint<int64,59, true>("-0.999999999999999999999999")).getBase(), int64(-1) << 59);

            EXPECT_EQ((FixedPoint<int64,63, true>("-0.27238237")).getBase(), -157017745924389449 << 4);
            EXPECT_EQ((FixedPoint<int64,62, true>("-0.27238237")).getBase(), -157017745924389449 << 3);
            EXPECT_EQ((FixedPoint<int64,61, true>("-0.27238237")).getBase(), -157017745924389449 << 2);
            EXPECT_EQ((FixedPoint<int64,60, true>("-0.27238237")).getBase(), -157017745924389449 << 1);
            EXPECT_EQ((FixedPoint<int64,59, true>("-0.27238237")).getBase(), -157017745924389449);

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
        
        TEST(FixedPointConstructor, radianConstructor) {
            EXPECT_EQ((000.0009938292205333709716796875_fxr).getBase(), 2134232);
            EXPECT_EQ((0.0393890053965151309967041015625_fxr).getBase(), 84587245);
            EXPECT_EQ((0.0000000004656612873077392578125_fxr).getBase(), 1);
            EXPECT_EQ((1.32012755982577800750732421875_fxr).getBase(), 2834952348);
            EXPECT_EQ((1.833883224986493587493896484375_fxr).getBase(), 3938234238);

            EXPECT_EQ((00.2738421123_fxr).getBase(), 588071458);
            EXPECT_EQ((00001.565342_fxr).getBase(), 3361546349);
            EXPECT_EQ((000.893423_fxr).getBase(), 1918611283);
        }

        TEST(FixedPointConstructor, radianOverflowConstructor) {
            EXPECT_EQ((2.0000000004656612873077392578125_fxr).getBase(), 1);
            EXPECT_EQ((3.32012755982577800750732421875_fxr).getBase(), 2834952348);
            EXPECT_EQ((1.9999999999999999999999999_fxr).getBase(), 0);
            EXPECT_EQ((2134.893423_fxr).getBase(), 1918611283);
            EXPECT_EQ((4243235235239.32012755982577800750732421875_fxr).getBase(), 2834952348);
            EXPECT_EQ((84593983459834893298289289339289432.0393890053965151309967041015625_fxr).getBase(), 84587245);
            EXPECT_EQ((09094322903490234093290309009092209221.833883224986493587493896484375_fxr).getBase(), 3938234238);
        }

        TEST(FixedPointConstructor, generalOverflowConstructor) {
            EXPECT_EQ((FixedPoint<uint64, 8, false>("72057594037927935.999999999")).getBase(), 0);
            EXPECT_EQ((FixedPoint<uint8, 3, false>("32.375")).getBase(), 3);
            EXPECT_EQ((FixedPoint<uint16, 5, false>("23423423423.71875")).getBase(), 30711);
        }
        
        TEST(FixedPointConstructor, mixedFixedPointConstructor) {
            EXPECT_EQ((2342.24782_fx).getBase(), 599615);
            EXPECT_EQ((1231.35498_fx).getBase(), 315227);
            EXPECT_EQ((-234.35559_fx).getBase(), -59995);

            EXPECT_EQ((23238421.213412_fxd).getBase(), 47592286645);
            EXPECT_EQ((8.23192381_fxd).getBase(), 16859);
            EXPECT_EQ((4503599627370495.99951171875_fxd).getBase(), 9223372036854775807);
            EXPECT_EQ((doubleFixed("-4503599627370496.000000")).getBase(), -9223372036854775807 - 1);
            EXPECT_EQ((fixed("-8388608.000000")).getBase(), -2147483648);

            EXPECT_EQ((FixedPoint<int16,5, true>("3.234198")).getBase(), 103);
            EXPECT_EQ((FixedPoint<int8,7, true>("0.848352")).getBase(), 109);
        }
        
        TEST(FixedPointConstructor, leadingZero) {
            EXPECT_EQ((000000000000000002342.24782_fx).getBase(), 599615);
            EXPECT_EQ((00000000001231.35498_fx).getBase(), 315227);
            EXPECT_EQ((fixed("-.99609375")).getBase(), -255);
            EXPECT_EQ((fixed("-00000000.37109375")).getBase(), -95);
            EXPECT_EQ((fixed("-0000.16406250000000")).getBase(), -42);
            EXPECT_EQ((fixed("-0000000000000000000.00390625")).getBase(), -1);
        }

        TEST(FixedPointConstructor, reachAssertFailOutofBounds) {
            #if DEBUGGING
            GTEST_FLAG_SET(death_test_style, "threadsafe");

            EXPECT_DEATH(doubleFixed("-4503599627370496.000250"),".*");
            EXPECT_DEATH(fixed("-8388608.004250"),".*");
            EXPECT_DEATH(4503599627370495.999999999999_fxd, ".*");
            EXPECT_DEATH(9000000000000000_fxd, ".*");
            EXPECT_DEATH(19000000023423400000_fxd, ".*");
            EXPECT_DEATH(8388607.99999999_fx, ".*");
            EXPECT_DEATH(radian("-1"), ".*");
            EXPECT_DEATH(radian("-0.2342342342"), ".*");

            EXPECT_DEATH((FixedPoint<uint8, 6, true>("4")), ".*");
            EXPECT_DEATH((FixedPoint<uint8, 6, true>("3.999999999999")), ".*");
            EXPECT_DEATH((FixedPoint<uint8, 7, true>("2")), ".*");
            EXPECT_DEATH((FixedPoint<uint8, 7, true>("1.999999999")), ".*");
            #endif
            EXPECT_TRUE(true);
        }

        TEST(FixedPointConstructor, reachAssertFailInvalidChar) {
            #if DEBUGGING
            GTEST_FLAG_SET(death_test_style, "threadsafe");

            EXPECT_DEATH(fixed("--"), ".*");
            EXPECT_DEATH(doubleFixed("-osidvso-"), ".*");
            EXPECT_DEATH(doubleFixed("2342342.12f"), ".*");
            EXPECT_DEATH(doubleFixed("23wef42342.12f"), ".*");
            EXPECT_DEATH(radian(""), ".*");
            EXPECT_DEATH(radian(".00."), ".*");
            EXPECT_DEATH(radian("23ersiofwef"), ".*");
            EXPECT_DEATH(radian("kantian1"), ".*");
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
    TEST(FixedPointCasting, sameFixedPointCast) {
        EXPECT_EQ(fixed(213123.2323_fx), 213123.2323_fx);
        EXPECT_EQ(fixed(213.58923_fx), 213.58923_fx);
        EXPECT_EQ(fixed(-213123.2323_fx), -213123.2323_fx);
        EXPECT_EQ(fixed(-213.58923_fx), -213.58923_fx);

        EXPECT_EQ(doubleFixed(13123.2323_fxd), 13123.2323_fxd);
        EXPECT_EQ(doubleFixed(213.58923_fxd), 213.58923_fxd);
        EXPECT_EQ(doubleFixed(-213123.2323_fxd), -213123.2323_fxd);
        EXPECT_EQ(doubleFixed(-213.58923_fxd), -213.58923_fxd);

        EXPECT_EQ(radian(0.234234_fxr), 0.234234_fxr);
        EXPECT_EQ(radian(1.5634_fxr), 1.5634_fxr);
        EXPECT_EQ(radian(0.854342_fxr), 0.854342_fxr);
        EXPECT_EQ(radian(1.2999_fxr), 1.2999_fxr);
    }

    TEST(FixedPointCasting, crossFixedPointCast) {

    }

    TEST(FixedPointCasting, crossFixedPointCastFailure) {

    }

    TEST(FixedPointCasting, signedIntegerCast) {
        EXPECT_EQ((int8)(43.1875_fx), 43);
        EXPECT_EQ((int8)(-2.6875_fx), -2);
        EXPECT_EQ((int8)(-32.1875_fx), -32);

        EXPECT_EQ((int16)(473.1875_fx), 473);
        EXPECT_EQ((int16)(-2.6875_fx), -2);
        EXPECT_EQ((int16)(-473.1875_fx), -473);

        EXPECT_EQ((int32)(473.1875_fx), 473);
        EXPECT_EQ((int32)(-2.6875_fx), -2);
        EXPECT_EQ((int32)(-473.1875_fx), -473);
        EXPECT_EQ((int32)(-121138.1875_fx), -121138);

        EXPECT_EQ((int8)(43.1875_fxd), 43);
        EXPECT_EQ((int8)(-2.6875_fxd), -2);
        EXPECT_EQ((int8)(-32.1875_fxd), -32);

        EXPECT_EQ((int16)(473.1875_fxd), 473);
        EXPECT_EQ((int16)(-2.6875_fxd), -2);
        EXPECT_EQ((int16)(-473.1875_fxd), -473);

        EXPECT_EQ((int32)(473.1875_fxd), 473);
        EXPECT_EQ((int32)(-2.6875_fxd), -2);
        EXPECT_EQ((int32)(-473.1875_fxd), -473);
        EXPECT_EQ((int32)(-121138.1875_fxd), -121138);

        EXPECT_EQ((int8)(1.1875_fxr), 1);
        EXPECT_EQ((int16)(0.1875_fxr), 0);
        EXPECT_EQ((int32)(1.544375_fxr), 1);
    }

    TEST(FixedPointCasting, unsignedIntegerCast) {
        EXPECT_EQ((uint8)(43.1875_fx), 43);
        EXPECT_EQ((uint16)(473.1875_fx), 473);
        EXPECT_EQ((uint32)(473.1875_fx), 473);
        EXPECT_EQ((uint8)(43.1875_fxd), 43);
        EXPECT_EQ((uint16)(473.1875_fxd), 473);
        EXPECT_EQ((uint32)(473.1875_fxd), 473);
        EXPECT_EQ((uint8)(1.1875_fxr), 1);
        EXPECT_EQ((uint16)(0.1875_fxr), 0);
        EXPECT_EQ((uint32)(1.1875_fxr), 1);
    }

    TEST(FixedPointCasting, signedCastFailure) {
        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(uint8 warnPrevent = (uint8)fixed("-43.1875");, ".*");
        EXPECT_DEATH(uint16 warnPrevent = (uint16)fixed("-473.1875");, ".*");
        EXPECT_DEATH(uint32 warnPrevent = (uint32)fixed("-473.1875");, ".*");
        EXPECT_DEATH(uint8 warnPrevent = (uint8)doubleFixed("-43.1875");, ".*");
        EXPECT_DEATH(uint16 warnPrevent = (uint16)doubleFixed("-473.1875");, ".*");
        EXPECT_DEATH(uint32 warnPrevent = (uint32)doubleFixed("-473.1875");, ".*");
        #endif
        EXPECT_TRUE(true);
    }

    TEST(FixedPointCasting, floatingPointCast) {
        EXPECT_EQ((float)(0_fx), 0.f);
        EXPECT_EQ((float)(2.6875_fx), 2.6875f);
        EXPECT_EQ((float)(473.1875_fx), 473.1875f);
        EXPECT_EQ((float)(-2.6875_fx), -2.6875f);
        EXPECT_EQ((float)(-473.1875_fx), -473.1875f);
        EXPECT_EQ((float)(-121138.1875_fx), -121138.1875f);
        EXPECT_EQ((float)(121138.1875_fx), 121138.1875f);
        EXPECT_EQ((float)(-0.71875_fx), -0.71875f);
        EXPECT_EQ((float)(0.71875_fx), 0.71875f);
        EXPECT_EQ((float)(0.71875_fx), 0.71875f);
        EXPECT_EQ((float)(8388607_fx), 8388607.0f);
        EXPECT_EQ((float)(0.71875_fx), 0.71875f);

        EXPECT_EQ((float)(0_fxd), 0);
        EXPECT_EQ((float)(2.6875_fxd), 2.6875f);
        EXPECT_EQ((float)(473.1875_fxd), 473.1875f);
        EXPECT_EQ((float)(-2.6875_fxd), -2.6875f);
        EXPECT_EQ((float)(-473.1875_fxd), -473.1875f);
        EXPECT_EQ((float)(-121138.1875_fxd), -121138.1875f);
        EXPECT_EQ((float)(121138.1875_fxd), 121138.1875f);
        EXPECT_EQ((float)(-0.71875_fxd), -0.71875f);
        EXPECT_EQ((float)(0.71875_fxd), 0.71875f);
        EXPECT_EQ((float)(4503599627370495_fxd), 4503599358935040.0f);
        EXPECT_EQ((float)doubleFixed("-4503599627370496"), -4503599627370496.0f);

        EXPECT_EQ((float)(FixedPoint<int16,5, true>("3.234198")), 3.21875f);
        EXPECT_EQ((float)(FixedPoint<int8,7, true>("0.848352")), 0.8515625f);

        EXPECT_EQ((float)(FixedPoint<int64,5, true>("3.234198")), 3.21875f);
        EXPECT_EQ((float)(FixedPoint<int64,7, true>("0.848352")), 0.8515625f);

        EXPECT_EQ((float)(FixedPoint<int64,59, true>("0.0009720075980754923350435348083919961936771869659423828125")), 0.0009720075759105384349822998046875f);
        EXPECT_EQ((float)(FixedPoint<int64,39, true>("1019.22383915960745071060955524444580078125")), 1019.22381591796875f);

        EXPECT_EQ((float)(000.125_fxr), 0.125f);
        EXPECT_EQ((float)(0.015625_fxr), 0.015625f);
        EXPECT_EQ((float)(0.4140625_fxr), 0.4140625f);
        EXPECT_EQ((float)(0.90625_fxr), .90625f);
        EXPECT_EQ((float)(1.265625_fxr), 1.265625f);
        EXPECT_EQ((float)(1.734375_fxr), 1.734375f);
    }

    TEST(FixedPointCasting, doubleFloatingPointCast) {
        EXPECT_EQ((double)(0_fx), 0);
        EXPECT_EQ((double)(2.6875_fx), 2.6875);
        EXPECT_EQ((double)(473.1875_fx), 473.1875);
        EXPECT_EQ((double)(-2.6875_fx), -2.6875);
        EXPECT_EQ((double)(-473.1875_fx), -473.1875);
        EXPECT_EQ((double)(-121138.1875_fx), -121138.1875);
        EXPECT_EQ((double)(121138.1875_fx), 121138.1875);
        EXPECT_EQ((double)(-0.71875_fx), -0.71875);
        EXPECT_EQ((double)(0.71875_fx), 0.71875);
        EXPECT_EQ((double)(0.71875_fx), 0.71875);
        EXPECT_EQ((double)(182.023112_fx), 182.0234375);
        EXPECT_EQ((double)(8388607_fx), 8388607.0);

        EXPECT_EQ((double)(0_fxd), 0);
        EXPECT_EQ((double)(0.11212121_fxd), 0.1123046875);
        EXPECT_EQ((double)(2.6875_fxd), 2.6875);
        EXPECT_EQ((double)(473.1875_fxd), 473.1875);
        EXPECT_EQ((double)(-2.6875_fxd), -2.6875);
        EXPECT_EQ((double)(-473.1875_fxd), -473.1875);
        EXPECT_EQ((double)(-121138.1875_fxd), -121138.1875);
        EXPECT_EQ((double)(121138.1875_fxd), 121138.1875);
        EXPECT_EQ((double)(-0.71875_fxd), -0.71875);
        EXPECT_EQ((double)(0.71875_fxd), 0.71875);
        EXPECT_EQ((double)(4503599627370494_fxd), 4503599627370494.0);
        EXPECT_EQ((double)doubleFixed("-4503599627370496"), -4503599627370496.0);

        EXPECT_EQ((double)(FixedPoint<int16,5, true>("3.234198")), 3.21875);
        EXPECT_EQ((double)(FixedPoint<int8,7, true>("0.848352")), 0.8515625);

        EXPECT_EQ((double)(FixedPoint<int64, 5, true>("3.234198")), 3.21875);
        EXPECT_EQ((double)(FixedPoint<int64, 7, true>("0.848352")), 0.8515625);

        EXPECT_EQ((double)(FixedPoint<int64,59, true>("0.0009720075980754923350435348083919961936771869659423828125")), 0.0009720075980754923350435348083919961936771869659423828125);
        EXPECT_EQ((double)(FixedPoint<int64,39, true>("1019.22383915960745071060955524444580078125")), 1019.22383915960745071060955524444580078125);

        EXPECT_EQ((double)(000.125_fxr), 0.125);
        EXPECT_EQ((double)(0.015625_fxr), 0.015625);
        EXPECT_EQ((double)(0.4140625_fxr), 0.4140625);
        EXPECT_EQ((double)(0.90625_fxr), .90625);
        EXPECT_EQ((double)(1.265625_fxr), 1.265625);
        EXPECT_EQ((double)(1.734375_fxr), 1.734375);
    }
#pragma endregion

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}