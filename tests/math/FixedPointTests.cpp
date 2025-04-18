#include <gtest/gtest.h>

#include "FixedPoint.hpp"

// I previously change radians from uint32,31,false to uint16,15,false. 
// There are a lot of tests revolving around FixedPoint<uint32,31,false> as a result however.

// Tests if constructors lead to proper values
#pragma region Constructor
    TEST(FixedPointConstructor, emptyConstructor) {
        EXPECT_EQ(fixed().getData(), 0);

        EXPECT_EQ(doubleFixed().getData(), 0);

        EXPECT_EQ(radian().getData(), 0);
    }

    TEST(FixedPointConstructor, integerConstructor) {
        EXPECT_EQ(fixed(23).getData(), 23 << 8);
        EXPECT_EQ(fixed(3).getData(), 3 << 8);
        EXPECT_EQ(fixed(-23).getData(), -23 << 8);
        EXPECT_EQ(fixed(-3).getData(), -3 << 8);

        EXPECT_EQ(doubleFixed(23).getData(), 23 << 11);
        EXPECT_EQ(doubleFixed(3).getData(), 3 << 11);
        EXPECT_EQ(doubleFixed(-23).getData(), -23 << 11);
        EXPECT_EQ(doubleFixed(-3).getData(), -3 << 11);

        EXPECT_EQ(radian(34523412).getData(), 0);
        EXPECT_EQ(radian(123123).getData(), 1 << 15);
        EXPECT_EQ(radian(4).getData(), 0);
        EXPECT_EQ(radian(3).getData(), 1 << 15);
        EXPECT_EQ(radian(2).getData(), 0);
        EXPECT_EQ(radian(1).getData(), 1 << 15);
        EXPECT_EQ(radian(0).getData(), 0);
    }

    TEST(FixedPointConstructor, integerConstructorFail) {

        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(fixed(9032230).getData(), ".*");
        EXPECT_DEATH(fixed(8388608).getData(), ".*");
        EXPECT_EQ(fixed(8388607).getData(), 8388607 << 8);
        EXPECT_EQ(fixed(-8388608).getData(), -8388608 << 8);
        EXPECT_DEATH(fixed(-8388609).getData(), ".*");
        EXPECT_DEATH(fixed(-9422339).getData(), ".*");
        
        EXPECT_DEATH(doubleFixed(89324983289499995).getData(), ".*");
        EXPECT_DEATH(doubleFixed(4503599627370496).getData(), ".*");
        EXPECT_EQ(doubleFixed(4503599627370495).getData(), 4503599627370495 << 11);
        EXPECT_EQ(doubleFixed(-4503599627370496).getData(), -4503599627370496 << 11);
        EXPECT_DEATH(doubleFixed(-4503599629999999).getData(), ".*");
        EXPECT_DEATH(doubleFixed(-4503599627370497).getData(), ".*");

        EXPECT_EQ(radian(0).getData(), 0);
        EXPECT_DEATH(radian(-1).getData(), ".*");
        EXPECT_DEATH(radian(-2).getData(), ".*");
        
        #endif
        EXPECT_TRUE(true);
    }

    #pragma region Copy/Move Constructor
        TEST(FixedPointConstructor, sameFixedPointCast) {
            EXPECT_EQ(fixed(213123.2323_fx).getData(), (213123.2323_fx).getData());
            EXPECT_EQ(fixed(213.58923_fx).getData(), (213.58923_fx).getData());
            EXPECT_EQ(fixed(-213123.2323_fx).getData(), (-213123.2323_fx).getData());
            EXPECT_EQ(fixed(-213.58923_fx).getData(), (-213.58923_fx).getData());

            EXPECT_EQ(doubleFixed(13123.2323_fxd).getData(), (13123.2323_fxd).getData());
            EXPECT_EQ(doubleFixed(213.58923_fxd).getData(), (213.58923_fxd).getData());
            EXPECT_EQ(doubleFixed(-213123.2323_fxd).getData(), (-213123.2323_fxd).getData());
            EXPECT_EQ(doubleFixed(-213.58923_fxd).getData(), (-213.58923_fxd).getData());

            EXPECT_EQ((FixedPoint<uint32, 31, false>(FixedPoint<uint32, 31, false>("0.234234"))).getData(), (FixedPoint<uint32, 31, false>("0.234234")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(FixedPoint<uint32, 31, false>("1.5634"))).getData(), (FixedPoint<uint32, 31, false>("1.5634")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(FixedPoint<uint32, 31, false>("0.854342"))).getData(), (FixedPoint<uint32, 31, false>("0.854342")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(FixedPoint<uint32, 31, false>("1.2999"))).getData(), (FixedPoint<uint32, 31, false>("1.2999")).getData());

            EXPECT_EQ((radian(radian("0.234234"))).getData(), (radian("0.234234")).getData());
            EXPECT_EQ((radian(radian("1.5634"))).getData(), (radian("1.5634")).getData());
            EXPECT_EQ((radian(radian("0.854342"))).getData(), (radian("0.854342")).getData());
            EXPECT_EQ((radian(radian("1.2999"))).getData(), (radian("1.2999")).getData());
        }

        TEST(FixedPointConstructor, crossFixedPointExactTest) {
            EXPECT_EQ(doubleFixed(473.1875_fx).getData(), (473.1875_fxd).getData());
            EXPECT_EQ(doubleFixed(43.1875_fx).getData(), (43.1875_fxd).getData());
            EXPECT_EQ(doubleFixed(-2.6875_fx).getData(), (-2.6875_fxd).getData());
            EXPECT_EQ(doubleFixed(-32.1875_fx).getData(), (-32.1875_fxd).getData());

            EXPECT_EQ(fixed(473.1875_fxd).getData(), (473.1875_fx).getData());
            EXPECT_EQ(fixed(43.1875_fxd).getData(), (43.1875_fx).getData());
            EXPECT_EQ(fixed(-2.6875_fxd).getData(), (-2.6875_fx).getData());
            EXPECT_EQ(fixed(-32.1875_fxd).getData(), (-32.1875_fx).getData());

            EXPECT_EQ((FixedPoint<uint32, 31, false>(0.37109375_fxd)).getData(), (FixedPoint<uint32, 31, false>("0.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(1.99609375_fxd)).getData(), (FixedPoint<uint32, 31, false>("1.99609375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(0.6875_fxd)).getData(), (FixedPoint<uint32, 31, false>("0.6875")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(1.1875_fxd)).getData(), (FixedPoint<uint32, 31, false>("1.1875")).getData());

            EXPECT_EQ((FixedPoint<uint32, 31, false>(0.37109375_fx)).getData(), (FixedPoint<uint32, 31, false>("0.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(1.99609375_fx)).getData(), (FixedPoint<uint32, 31, false>("1.99609375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(0.6875_fx)).getData(), (FixedPoint<uint32, 31, false>("0.6875")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(1.1875_fx)).getData(), (FixedPoint<uint32, 31, false>("1.1875")).getData());

            EXPECT_EQ((radian(0.37109375_fxd)).getData(), (radian("0.37109375")).getData());
            EXPECT_EQ((radian(1.99609375_fxd)).getData(), (radian("1.99609375")).getData());
            EXPECT_EQ((radian(0.6875_fxd)).getData(), (radian("0.6875")).getData());
            EXPECT_EQ((radian(1.1875_fxd)).getData(), (radian("1.1875")).getData());

            EXPECT_EQ((radian(0.37109375_fx)).getData(), (radian("0.37109375")).getData());
            EXPECT_EQ((radian(1.99609375_fx)).getData(), (radian("1.99609375")).getData());
            EXPECT_EQ((radian(0.6875_fx)).getData(), (radian("0.6875")).getData());
            EXPECT_EQ((radian(1.1875_fx)).getData(), (radian("1.1875")).getData());
        }

        TEST(FixedPointConstructor, crossFixedPointOverflow) {
            EXPECT_EQ((FixedPoint<uint32, 31, false>(12312231.37109375_fxd)).getData(), (FixedPoint<uint32, 31, false>("1.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(39081.25_fxd)).getData(), (FixedPoint<uint32, 31, false>("1.25")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(12312232.37109375_fxd)).getData(), (FixedPoint<uint32, 31, false>("0.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(39084.25_fxd)).getData(), (FixedPoint<uint32, 31, false>("0.25")).getData());

            EXPECT_EQ((FixedPoint<uint32, 31, false>(223.37109375_fx)).getData(), (FixedPoint<uint32, 31, false>("1.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(39081.25_fx)).getData(), (FixedPoint<uint32, 31, false>("1.25")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(1212.37109375_fx)).getData(), (FixedPoint<uint32, 31, false>("0.37109375")).getData());
            EXPECT_EQ((FixedPoint<uint32, 31, false>(39084.25_fx)).getData(), (FixedPoint<uint32, 31, false>("0.25")).getData());

            EXPECT_EQ((radian(12312231.37109375_fxd)).getData(), (radian("1.37109375")).getData());
            EXPECT_EQ((radian(39081.25_fxd)).getData(), (radian("1.25")).getData());
            EXPECT_EQ((radian(12312232.37109375_fxd)).getData(), (radian("0.37109375")).getData());
            EXPECT_EQ((radian(39084.25_fxd)).getData(), (radian("0.25")).getData());

            EXPECT_EQ((radian(223.37109375_fx)).getData(), (radian("1.37109375")).getData());
            EXPECT_EQ((radian(39081.25_fx)).getData(), (radian("1.25")).getData());
            EXPECT_EQ((radian(1212.37109375_fx)).getData(), (radian("0.37109375")).getData());
            EXPECT_EQ((radian(39084.25_fx)).getData(), (radian("0.25")).getData());

            EXPECT_EQ((FixedPoint<uint8, 4, false>(123221.125_fxd)).getData(), (FixedPoint<uint8, 4, false>("5.125")).getData());
            EXPECT_EQ((FixedPoint<uint32, 8, false>(939123233231242.00390625_fxd)).getData(), (FixedPoint<uint32, 8, false>("12030346.00390625")).getData());
            EXPECT_EQ((FixedPoint<uint16, 16, false>(39084_fxd)).getData(), (FixedPoint<uint16, 16, false>("0")).getData());
            EXPECT_EQ((FixedPoint<uint16, 15, false>(39084.125_fxd)).getData(), (FixedPoint<uint16, 15, false>("0.125")).getData());
        }

        TEST(FixedPointConstructor, crossFixedPointPrecisionLoss) {
            EXPECT_EQ(fixed(2362.9599609375_fxd).getData(), (2362.95703125_fx).getData());
            EXPECT_EQ(fixed(-2362.9599609375_fxd).getData(), (-2362.95703125_fx).getData());

            EXPECT_EQ(fixed(2.0126953125_fxd).getData(), (2.01171875_fx).getData());
            EXPECT_EQ(fixed(-2.0126953125_fxd).getData(), (-2.01171875_fx).getData());

            EXPECT_EQ(fixed(21891.06591796875_fxd).getData(), (21891.0625_fx).getData());
            EXPECT_EQ(fixed(-21891.06591796875_fxd).getData(), (-21891.0625_fx).getData());

            EXPECT_EQ(fixed(1.367279052734375_fxr).getData(), (1.3671875_fx).getData());
            EXPECT_EQ(fixed(0.2054443359375_fxr).getData(), (0.203125_fx).getData());
            EXPECT_EQ(fixed(0.713714599609375_fxr).getData(), (0.7109375_fx).getData());
        }

        TEST(FixedPointConstructor, crossFixedPointCastFailure) {
            #if DEBUGGING
            EXPECT_DEATH(fixed(-8388608.00390625_fxd).getData(), ".*");
            EXPECT_EQ(fixed(-8388608_fxd).getData(), fixed("-8388608").getData());

            EXPECT_DEATH(fixed(8388607.99999999_fxd).getData(), ".*");
            EXPECT_EQ(fixed(8388607.99609375_fxd).getData(), (8388607.99609375_fx).getData());

            EXPECT_DEATH((FixedPoint<int16, 10>(1024_fx).getData()), ".*");
            EXPECT_DEATH((FixedPoint<int16, 10>(1023.99609375_fx).getData()), ".*");

            EXPECT_DEATH((FixedPoint<int16, 10>(-1024_fx).getData()), ".*");
            EXPECT_DEATH((FixedPoint<int16, 10>(-1024.00390625_fx).getData()), ".*");

            EXPECT_DEATH((FixedPoint<uint32, 31, false>(-0.37109375_fxd)).getData(), ".*");
            EXPECT_DEATH((FixedPoint<uint32, 31, false>(-1.1875_fx)).getData(), ".*");

            EXPECT_DEATH((radian(-0.37109375_fxd)).getData(), ".*");
            EXPECT_DEATH((radian(-1.1875_fx)).getData(), ".*");
            #endif
            EXPECT_TRUE(true);
        }

    #pragma endregion

    #pragma region Char String Constructor
        // Negative number use constructor instead of literal due to literals ignoring negative values and just constructing it then applying -
        TEST(FixedPointConstructor, positiveIntegerConstructor) {
            EXPECT_EQ((0_fx).getData(), 0);
            EXPECT_EQ((0_fx).getData(), 0);
            EXPECT_EQ((312312_fx).getData(), (312312 << 8));
            EXPECT_EQ((2122_fx).getData(), (2122 << 8));
            EXPECT_EQ((22_fx).getData(), (22 << 8));
            EXPECT_EQ((2342232_fx).getData(), (2342232 << 8));
            EXPECT_EQ((8388607_fx).getData(), (8388607<< 8));
            EXPECT_EQ((312312_fxd).getData(), (uint64(312312) << 11));
            EXPECT_EQ((2122_fxd).getData(), (uint64(2122) << 11));
            EXPECT_EQ((22_fxd).getData(), (uint64(22) << 11));
            EXPECT_EQ((2342232_fxd).getData(), (uint64(2342232) << 11));
            EXPECT_EQ((8388607_fxd).getData(), (uint64(8388607)<< 11));
            EXPECT_EQ((8388607_fxd).getData(), (uint64(8388607)<< 11));
            EXPECT_EQ((934934993_fxd).getData(), (uint64(934934993u)<< 11));
            EXPECT_EQ((13976329829_fxd).getData(), (uint64(13976329829u)<< 11));
            EXPECT_EQ((23233248923982_fxd).getData(), (uint64(23233248923982u)<< 11));
            EXPECT_EQ((4503599627370495_fxd).getData(), (uint64(4503599627370495u)<< 11));
            EXPECT_EQ((FixedPoint<int32, 5, true>("53392309")).getData(), uint64(53392309u) << 5);
            EXPECT_EQ((FixedPoint<int8, 3, true>("5")).getData(), uint64(5u) << 3);
            EXPECT_EQ((FixedPoint<int64, 0, true>("9223372036854775807")).getData(), uint64(9223372036854775807u));
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0")).getData(), 0);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1")).getData(), (uint64(1) << 31));
            EXPECT_EQ((0_fxr).getData(), 0);
            EXPECT_EQ((1_fxr).getData(), (uint64(1) << 15));
        }

        TEST(FixedPointConstructor, negativeIntegerConstructor) {
            EXPECT_EQ((fixed("-312312")).getData(), -(312312 << 8));
            EXPECT_EQ((fixed("-2122")).getData(), -(2122 << 8));
            EXPECT_EQ((fixed("-22")).getData(), -(22 << 8));
            EXPECT_EQ((fixed("-2342232")).getData(), -(2342232 << 8));
            EXPECT_EQ((fixed("-8388607")).getData(), -(8388607<< 8));
            EXPECT_EQ((doubleFixed("-312312")).getData(), -(int64(312312) << 11));
            EXPECT_EQ((doubleFixed("-2122")).getData(), -(int64(2122) << 11));
            EXPECT_EQ((doubleFixed("-22")).getData(), -(int64(22) << 11));
            EXPECT_EQ((doubleFixed("-2342232")).getData(), -(int64(2342232) << 11));
            EXPECT_EQ((doubleFixed("-8388607")).getData(), -(int64(8388607)<< 11));
            EXPECT_EQ((doubleFixed("-8388607")).getData(), -(int64(8388607)<< 11));
            EXPECT_EQ((doubleFixed("-934934993")).getData(), -(int64(934934993u)<< 11));
            EXPECT_EQ((doubleFixed("-13976329829")).getData(), -(int64(13976329829u)<< 11));
            EXPECT_EQ((doubleFixed("-23233248923982")).getData(), -(int64(23233248923982u)<< 11));
            EXPECT_EQ((doubleFixed("-4503599627370495")).getData(), -(int64(4503599627370495u)<< 11));
            EXPECT_EQ((FixedPoint<int32, 5, true>("-53392309")).getData(), -(int64(53392309u) << 5));
            EXPECT_EQ((FixedPoint<int8, 3, true>("-5")).getData(), -(int64(5u) << 3));
            EXPECT_EQ((FixedPoint<int64, 0, true>("-9223372036854775807")).getData(), -int64(9223372036854775807u));
            EXPECT_EQ((FixedPoint<int64, 0, true>("-9223372036854775808")).getData(), std::numeric_limits<int64>::min());
        }

        TEST(FixedPointConstructor, exactPositiveDecimalConstructor) {
            EXPECT_EQ((0.9765625_fxd).getData(), 2000);
            EXPECT_EQ((0.68505859375_fxd).getData(), 1403);
            EXPECT_EQ((0.46240234375_fxd).getData(), 947);
            EXPECT_EQ((0.20654296875_fxd).getData(), 423);
            EXPECT_EQ((0.1142578125_fxd).getData(), 234);
            EXPECT_EQ((0.00048828125_fxd).getData(), 1);
            EXPECT_EQ((0.000_fxd).getData(), 0);

            EXPECT_EQ((0.99609375_fx).getData(), 255);
            EXPECT_EQ((0.37109375_fx).getData(), 95);
            EXPECT_EQ((0.1640625_fx).getData(), 42);
            EXPECT_EQ((0.00390625_fx).getData(), 1);
            EXPECT_EQ((0.000_fx).getData(), 0);

            EXPECT_EQ((0.999969482421875_fxr).getData(), 32767);
            EXPECT_EQ((0.64801025390625_fxr).getData(), 21234);
            EXPECT_EQ((0.291259765625_fxr).getData(), 9544);
            EXPECT_EQ((0.1630859375_fxr).getData(), 5344);
        }
        
        TEST(FixedPointConstructor, roundedPositiveDecimalConstructor) {

            EXPECT_EQ((FixedPoint<int64, 62, true>("0.999999999999999999999999")).getData(), uint64(1) << 62);
            EXPECT_EQ((FixedPoint<int64, 61, true>("0.999999999999999999999999")).getData(), uint64(1) << 61);
            EXPECT_EQ((FixedPoint<int64, 60, true>("0.999999999999999999999999")).getData(), uint64(1) << 60);
            EXPECT_EQ((FixedPoint<int64, 59, true>("0.999999999999999999999999")).getData(), uint64(1) << 59);

            EXPECT_EQ((FixedPoint<int64, 63, true>("0.27238237")).getData(), 157017745924389449 << 4);
            EXPECT_EQ((FixedPoint<int64, 62, true>("0.27238237")).getData(), 157017745924389449 << 3);
            EXPECT_EQ((FixedPoint<int64, 61, true>("0.27238237")).getData(), 157017745924389449 << 2);
            EXPECT_EQ((FixedPoint<int64, 60, true>("0.27238237")).getData(), 157017745924389449 << 1);
            EXPECT_EQ((FixedPoint<int64, 59, true>("0.27238237")).getData(), 157017745924389449);

            EXPECT_EQ((0.9999999_fxd).getData(), 2048);
            EXPECT_EQ((0.9943789_fxd).getData(), 2036);
            EXPECT_EQ((0.589432_fxd).getData(), 1207);
            EXPECT_EQ((0.2343598423_fxd).getData(), 480);
            EXPECT_EQ((0.61398_fxd).getData(), 1257);
            EXPECT_EQ((0.19424_fxd).getData(), 398);
            EXPECT_EQ((0.002_fxd).getData(), 4);
            EXPECT_EQ((0.001_fxd).getData(), 2);
            
            EXPECT_EQ((0.9999999_fx).getData(), 256);
            EXPECT_EQ((0.9943789_fx).getData(), 255);
            EXPECT_EQ((0.589432_fx).getData(), 151);
            EXPECT_EQ((0.2343598423_fx).getData(), 60);
            EXPECT_EQ((0.61398_fx).getData(), 157);
            EXPECT_EQ((0.19424_fx).getData(), 50);
            EXPECT_EQ((0.002_fx).getData(), 1);
            EXPECT_EQ((0.001_fx).getData(), 0);

            EXPECT_EQ((0.9999999_fxr).getData(), 32768);
            EXPECT_EQ((0.9943789_fxr).getData(), 32584);
            EXPECT_EQ((0.589432_fxr).getData(), 19315);
            EXPECT_EQ((0.2343598423_fxr).getData(), 7680);
            EXPECT_EQ((0.61398_fxr).getData(), 20119);
            EXPECT_EQ((0.19424_fxr).getData(), 6365);
            EXPECT_EQ((0.002_fxr).getData(), 66);
            EXPECT_EQ((0.001_fxr).getData(), 33);
        }
        
        TEST(FixedPointConstructor, exactNegativeDecimalConstructor) {
            EXPECT_EQ((doubleFixed("-0.9765625")).getData(), -2000);
            EXPECT_EQ((doubleFixed("-0.68505859375")).getData(), -1403);
            EXPECT_EQ((doubleFixed("-0.46240234375")).getData(), -947);
            EXPECT_EQ((doubleFixed("-0.20654296875")).getData(), -423);
            EXPECT_EQ((doubleFixed("-0.1142578125")).getData(), -234);
            EXPECT_EQ((doubleFixed("-0.00048828125")).getData(), -1);
            EXPECT_EQ((doubleFixed("-0.000")).getData(), -0);

            EXPECT_EQ((fixed("-0.99609375")).getData(), -255);
            EXPECT_EQ((fixed("-0.37109375")).getData(), -95);
            EXPECT_EQ((fixed("-0.1640625")).getData(), -42);
            EXPECT_EQ((fixed("-0.00390625")).getData(), -1);
            EXPECT_EQ((fixed("-0.000")).getData(), -0);
        }
        
        TEST(FixedPointConstructor, roundedNegativeDecimalConstructor) {

            EXPECT_EQ((FixedPoint<int64,62, true>("-0.999999999999999999999999")).getData(), int64(-1) << 62);
            EXPECT_EQ((FixedPoint<int64,61, true>("-0.999999999999999999999999")).getData(), int64(-1) << 61);
            EXPECT_EQ((FixedPoint<int64,60, true>("-0.999999999999999999999999")).getData(), int64(-1) << 60);
            EXPECT_EQ((FixedPoint<int64,59, true>("-0.999999999999999999999999")).getData(), int64(-1) << 59);

            EXPECT_EQ((FixedPoint<int64,63, true>("-0.27238237")).getData(), -157017745924389449 << 4);
            EXPECT_EQ((FixedPoint<int64,62, true>("-0.27238237")).getData(), -157017745924389449 << 3);
            EXPECT_EQ((FixedPoint<int64,61, true>("-0.27238237")).getData(), -157017745924389449 << 2);
            EXPECT_EQ((FixedPoint<int64,60, true>("-0.27238237")).getData(), -157017745924389449 << 1);
            EXPECT_EQ((FixedPoint<int64,59, true>("-0.27238237")).getData(), -157017745924389449);

            EXPECT_EQ((doubleFixed("-0.9999999")).getData(), -2048);
            EXPECT_EQ((doubleFixed("-0.9943789")).getData(), -2036);
            EXPECT_EQ((doubleFixed("-0.589432")).getData(), -1207);
            EXPECT_EQ((doubleFixed("-0.2343598423")).getData(), -480);
            EXPECT_EQ((doubleFixed("-0.61398")).getData(), -1257);
            EXPECT_EQ((doubleFixed("-0.19424")).getData(), -398);
            EXPECT_EQ((doubleFixed("-0.002")).getData(), -4);
            EXPECT_EQ((doubleFixed("-0.001")).getData(), -2);
            
            EXPECT_EQ((fixed("-0.9999999")).getData(), -256);
            EXPECT_EQ((fixed("-0.9943789")).getData(), -255);
            EXPECT_EQ((fixed("-0.589432")).getData(), -151);
            EXPECT_EQ((fixed("-0.2343598423")).getData(), -60);
            EXPECT_EQ((fixed("-0.61398")).getData(), -157);
            EXPECT_EQ((fixed("-0.19424")).getData(), -50);
            EXPECT_EQ((fixed("-0.002")).getData(), -1);
            EXPECT_EQ((fixed("-0.001")).getData(), -0);
        }
        
        TEST(FixedPointConstructor, oldRadianConstructor) {
            EXPECT_EQ((FixedPoint<uint32, 31, false>("000.0009938292205333709716796875")).getData(), 2134232);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0.0393890053965151309967041015625")).getData(), 84587245);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0.0000000004656612873077392578125")).getData(), 1);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1.32012755982577800750732421875")).getData(), 2834952348);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1.833883224986493587493896484375")).getData(), 3938234238);

            EXPECT_EQ((FixedPoint<uint32, 31, false>("00.2738421123")).getData(), 588071458);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("00001.565342")).getData(), 3361546349);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("000.893423")).getData(), 1918611283);
        }

        TEST(FixedPointConstructor, oldRadianOverflowConstructor) {
            EXPECT_EQ((FixedPoint<uint32, 31, false>("2.0000000004656612873077392578125")).getData(), 1);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("3.32012755982577800750732421875")).getData(), 2834952348);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1.9999999999999999999999999")).getData(), 0);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("2134.893423")).getData(), 1918611283);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("4243235235239.32012755982577800750732421875")).getData(), 2834952348);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("84593983459834893298289289339289432.0393890053965151309967041015625")).getData(), 84587245);
            EXPECT_EQ((FixedPoint<uint32, 31, false>("09094322903490234093290309009092209221.833883224986493587493896484375")).getData(), 3938234238);
        }

        TEST(FixedPointConstructor, radianOverflowConstructor) {
            EXPECT_EQ((radian("2.000030517578125")).getData(), 1);
            EXPECT_EQ((radian("3.037322998046875")).getData(), 33991);
            EXPECT_EQ((radian("1.9999999999999999999999999")).getData(), 0);
            EXPECT_EQ((radian("2134.893423")).getData(), 29276);
            EXPECT_EQ((radian("4243235235239.32012755982577800750732421875")).getData(), 43258);
            EXPECT_EQ((radian("84593983459834893298289289339289432.0393890053965151309967041015625")).getData(), 1291);
            EXPECT_EQ((radian("09094322903490234093290309009092209221.833883224986493587493896484375")).getData(), 60093);
        }


        TEST(FixedPointConstructor, generalOverflowConstructor) {
            EXPECT_EQ((FixedPoint<uint64, 8, false>("72057594037927935.999999999")).getData(), 0);
            EXPECT_EQ((FixedPoint<uint8, 3, false>("32.375")).getData(), 3);
            EXPECT_EQ((FixedPoint<uint16, 5, false>("23423423423.71875")).getData(), 30711);
        }
        
        TEST(FixedPointConstructor, mixedFixedPointConstructor) {
            EXPECT_EQ((2342.24782_fx).getData(), 599615);
            EXPECT_EQ((1231.35498_fx).getData(), 315227);
            EXPECT_EQ((-234.35559_fx).getData(), -59995);

            EXPECT_EQ((23238421.213412_fxd).getData(), 47592286645);
            EXPECT_EQ((8.23192381_fxd).getData(), 16859);
            EXPECT_EQ((4503599627370495.99951171875_fxd).getData(), 9223372036854775807);
            EXPECT_EQ((doubleFixed("-4503599627370496.000000")).getData(), -9223372036854775807 - 1);
            EXPECT_EQ((fixed("-8388608.000000")).getData(), -2147483648);

            EXPECT_EQ((FixedPoint<int16,5, true>("3.234198")).getData(), 103);
            EXPECT_EQ((FixedPoint<int8,7, true>("0.848352")).getData(), 109);

            EXPECT_EQ((radian("1.563201904296875")).getData(), 51223);
            EXPECT_EQ((radian("1.308990478515625")).getData(), 42893);
            EXPECT_EQ((radian("1.196990966796875")).getData(), 39223);
        }
        
        TEST(FixedPointConstructor, leadingZero) {
            EXPECT_EQ((000000000000000002342.24782_fx).getData(), 599615);
            EXPECT_EQ((00000000001231.35498_fx).getData(), 315227);
            EXPECT_EQ((fixed("-.99609375")).getData(), -255);
            EXPECT_EQ((fixed("-00000000.37109375")).getData(), -95);
            EXPECT_EQ((fixed("-0000.16406250000000")).getData(), -42);
            EXPECT_EQ((fixed("-0000000000000000000.00390625")).getData(), -1);
        }

        TEST(FixedPointConstructor, reachAssertFailOutofBounds) {
            #if DEBUGGING
            GTEST_FLAG_SET(death_test_style, "threadsafe");

            EXPECT_DEATH(fixed("-1231388609"),".*");
            EXPECT_DEATH(fixed("-8388609"),".*");
            EXPECT_DEATH(fixed("-8388608.004250"),".*");
            EXPECT_EQ(fixed("-8388608").getData(), -2147483648);

            EXPECT_EQ((8388607.99609375_fx).getData(), 2147483647);
            EXPECT_DEATH(8388607.99999999_fx, ".*");
            EXPECT_DEATH(8388608_fx, ".*");
            EXPECT_DEATH(122311288608_fx, ".*");

            EXPECT_DEATH(4503599627370496_fxd, ".*");
            EXPECT_DEATH(4503599627370495.999999999999_fxd, ".*");
            EXPECT_EQ((4503599627370495.99951171875_fxd).getData(), 9223372036854775807);

            EXPECT_EQ(doubleFixed("-4503599627370496").getData(), -9223372036854775807 - 1);
            EXPECT_DEATH(doubleFixed("-4503599627370496.000250"),".*");
            EXPECT_DEATH(doubleFixed("-4503599627370497"),".*");
            EXPECT_DEATH(doubleFixed("-31234310991390902"),".*");

            EXPECT_DEATH(9000000000000000_fxd, ".*");
            EXPECT_DEATH(19000000023423400000_fxd, ".*");


            EXPECT_DEATH((radian("-1")), ".*");
            EXPECT_DEATH((radian("-0.2342342342")), ".*");

            EXPECT_DEATH((FixedPoint<uint32, 31, false>("-1")), ".*");
            EXPECT_DEATH((FixedPoint<uint32, 31, false>("-0.2342342342")), ".*");

            EXPECT_DEATH((FixedPoint<uint64, 64, true>("2.23423")), ".*");
            EXPECT_DEATH((FixedPoint<uint64, 64, true>("1")), ".*");
            EXPECT_DEATH((FixedPoint<uint64, 64, true>("0.99999999999999999999999999")), ".*");
            EXPECT_EQ((FixedPoint<uint64, 64, true>("0.99999999999999999826527652402319290558807551860809326171875")).getData(), uint64(18446744073709551584ull));

            EXPECT_DEATH((FixedPoint<uint8, 6, true>("4")), ".*");
            EXPECT_DEATH((FixedPoint<uint8, 6, true>("3.999999999999")), ".*");
            EXPECT_EQ((FixedPoint<uint8, 6, true>("3.984375")).getData(), 255);

            EXPECT_EQ((FixedPoint<uint8, 6, true>("0")).getData(), 0);
            EXPECT_DEATH((FixedPoint<uint8, 6, true>("-0.111")), ".*");


            EXPECT_DEATH((FixedPoint<uint8, 7, true>("2")), ".*");
            EXPECT_DEATH((FixedPoint<uint8, 7, true>("1.999999999")), ".*");
            EXPECT_EQ((FixedPoint<uint8, 7, true>("1.9921875")).getData(), 255);

            EXPECT_EQ((FixedPoint<uint8, 7, true>("0")).getData(), 0);
            EXPECT_DEATH((FixedPoint<uint8, 7, true>("-0.111")), ".*");
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
            EXPECT_DEATH((FixedPoint<uint32, 31, false>("")), ".*");
            EXPECT_DEATH((FixedPoint<uint32, 31, false>(".00.")), ".*");
            EXPECT_DEATH((FixedPoint<uint32, 31, false>("23ersiofwef")), ".*");
            EXPECT_DEATH((FixedPoint<uint32, 31, false>("kantian1")), ".*");
            EXPECT_DEATH((radian("")), ".*");
            EXPECT_DEATH((radian(".00.")), ".*");
            EXPECT_DEATH((radian("23ersiofwef")), ".*");
            EXPECT_DEATH((radian("kantian1")), ".*");
            #endif
            EXPECT_TRUE(true);
        }
    #pragma endregion
#pragma endregion

// Tests if the string representation of chars work
# pragma region String Representation
        TEST(FixedPointStringRep, stringConversionInteger) {
            EXPECT_EQ((0_fx).toString(), "0");
            EXPECT_EQ((23423_fx).toString(), "23423");
            EXPECT_EQ((123123_fx).toString(), "123123");
            EXPECT_EQ((5232_fx).toString(), "5232");
            EXPECT_EQ((8388607_fx).toString(), "8388607");

            EXPECT_EQ((-23423_fx).toString(), "-23423");
            EXPECT_EQ((-123123_fx).toString(), "-123123");
            EXPECT_EQ((-5232_fx).toString(), "-5232");
            EXPECT_EQ(fixed("-8388608").toString(), "-8388608");

            EXPECT_EQ((0_fxd).toString(), "0");
            EXPECT_EQ((23423_fxd).toString(), "23423");
            EXPECT_EQ((123123_fxd).toString(), "123123");
            EXPECT_EQ((5232_fxd).toString(), "5232");
            EXPECT_EQ((8388607_fxd).toString(), "8388607");
            EXPECT_EQ((42323323_fxd).toString(), "42323323");
            EXPECT_EQ((4503599627370495_fxd).toString(), "4503599627370495");

            EXPECT_EQ((-23423_fxd).toString(), "-23423");
            EXPECT_EQ((-123123_fxd).toString(), "-123123");
            EXPECT_EQ((-5232_fxd).toString(), "-5232");
            EXPECT_EQ((-8388608_fxd).toString(), "-8388608");
            EXPECT_EQ((-42323323_fxd).toString(), "-42323323");
            EXPECT_EQ(doubleFixed("-4503599627370496").toString(), "-4503599627370496");

            EXPECT_EQ((FixedPoint<uint32, 31, false>("1")).toString(), "1");
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0")).toString(), "0");

            EXPECT_EQ((radian("1")).toString(), "1");
            EXPECT_EQ((radian("0")).toString(), "0");
        }

        TEST(FixedPointStringRep, stringConversionExactDecimal) {
            EXPECT_EQ((0.99609375_fx).toString(), "0.99609375");
            EXPECT_EQ((0.125_fx).toString(), "0.125");
            EXPECT_EQ((0.12890625_fx).toString(), "0.12890625");
            EXPECT_EQ((0.046875_fx).toString(), "0.046875");
            EXPECT_EQ((0.00390625_fx).toString(), "0.00390625");

            EXPECT_EQ((-0.99609375_fx).toString(), "-0.99609375");
            EXPECT_EQ((-0.125_fx).toString(), "-0.125");
            EXPECT_EQ((-0.12890625_fx).toString(), "-0.12890625");
            EXPECT_EQ((-0.046875_fx).toString(), "-0.046875");
            EXPECT_EQ((-0.00390625_fx).toString(), "-0.00390625");

            EXPECT_EQ((0.99951171875_fxd).toString(), "0.99951171875");
            EXPECT_EQ((0.99609375_fxd).toString(), "0.99609375");
            EXPECT_EQ((0.125_fxd).toString(), "0.125");
            EXPECT_EQ((0.1142578125_fxd).toString(), "0.1142578125");
            EXPECT_EQ((0.12890625_fxd).toString(), "0.12890625");
            EXPECT_EQ((0.046875_fxd).toString(), "0.046875");
            EXPECT_EQ((0.00390625_fxd).toString(), "0.00390625");
            EXPECT_EQ((0.00048828125_fxd).toString(), "0.00048828125");

            EXPECT_EQ((-0.99951171875_fxd).toString(), "-0.99951171875");
            EXPECT_EQ((-0.99609375_fxd).toString(), "-0.99609375");
            EXPECT_EQ((-0.125_fxd).toString(), "-0.125");
            EXPECT_EQ((-0.1142578125_fxd).toString(), "-0.1142578125");
            EXPECT_EQ((-0.12890625_fxd).toString(), "-0.12890625");
            EXPECT_EQ((-0.046875_fxd).toString(), "-0.046875");
            EXPECT_EQ((-0.00390625_fxd).toString(), "-0.00390625");
            EXPECT_EQ((-0.00048828125_fxd).toString(), "-0.00048828125");

            EXPECT_EQ((1.999969482421875_fxr).toString(), "1.999969482421875");
            EXPECT_EQ((0.714813232421875_fxr).toString(), "0.714813232421875");
            EXPECT_EQ((1.294952392578125_fxr).toString(), "1.294952392578125");
            EXPECT_EQ((0.001312255859375_fxr).toString(), "0.001312255859375");
            EXPECT_EQ((0.000030517578125_fxr).toString(), "0.000030517578125");

        }

        TEST(FixedPointStringRep, stringConversionApproximateDecimalOldRadian) {
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1.9999999995343387126922607421875")).toString(), "1.999996185302734375");
            EXPECT_EQ((FixedPoint<uint32, 31, false>("1.50973500311374664306640625")).toString(), "1.509735107421875");
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0.267951391637325286865234375")).toString(), "0.26795196533203125");
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0.057332796044647693634033203125")).toString(), "0.057331085205078125");
            EXPECT_EQ((FixedPoint<uint32, 31, false>("0.0000000004656612873077392578125")).toString(), "0");
        }

        TEST(FixedPointStringRep, stringConversionApproximateDecimal) {
            EXPECT_EQ((FixedPoint<uint64, 64, true>("0.99999999999999999826527652402319290558807551860809326171875")).toString(), "0.999996185302734375");
            EXPECT_EQ((FixedPoint<uint64, 64, true>("0.4950047118264213448436857067935790155388531275093555450439453125")).toString(), "0.495006561279296875");
            EXPECT_EQ((FixedPoint<uint64, 63, true>("1.99999999999999999826527652402319290558807551860809326171875")).toString(), "1.999996185302734375");
            EXPECT_EQ((FixedPoint<uint64, 63, true>("0.427114815286927934525999794868056369523401372134685516357421875")).toString(), "0.42711639404296875");
            EXPECT_EQ((FixedPoint<uint64, 32, true>("18.3444416332058608531951904296875")).toString(), "18.344440460205078125");

            EXPECT_EQ((FixedPoint<int32, 20, true>("2047.99999904632568359375")).toString(), "2047.999996185302734375");
            EXPECT_EQ((FixedPoint<int32, 20, true>("2033.43611907958984375")).toString(), "2033.43611907958984375");
            EXPECT_EQ((FixedPoint<int32, 20 , true>("-2033.43611907958984375")).toString(), "-2033.43611907958984375");
            EXPECT_EQ((FixedPoint<int32, 20, true>("-2048")).toString(), "-2048");

            EXPECT_EQ((FixedPoint<int64, 32, true>("2147483647.99999999976716935634613037109375")).toString(), "2147483647.999996185302734375");
            EXPECT_EQ((FixedPoint<int64, 32, true>("18.3444416332058608531951904296875")).toString(), "18.344440460205078125");
            EXPECT_EQ((FixedPoint<int64, 32, true>("-18.3444416332058608531951904296875")).toString(), "-18.344440460205078125");
            EXPECT_EQ((FixedPoint<int64, 32, true>("-2147483648")).toString(), "-2147483648");
        }
#pragma  endregion 

// Tests if equality between different 
#pragma region Comparison operations
    #pragma region Same type fixed point
        TEST(FixedPointComparison, FixedPointEqual) {
            EXPECT_TRUE(1.5_fx == 1.5_fx);
            EXPECT_TRUE((-3.25_fx) == (-3.25_fx));
            EXPECT_TRUE(0_fx == 0_fx);
            EXPECT_FALSE(1.5_fx == 2.5_fx);
            
            EXPECT_TRUE(1.5_fxd == 1.5_fxd);
            EXPECT_TRUE((-3.25_fxd) == (-3.25_fxd));
            
            EXPECT_TRUE(1.5_fxr == 1.5_fxr);
            EXPECT_TRUE(0.75_fxr == 0.75_fxr);
        }

        TEST(FixedPointComparison, FixedPointEqualEdgeCase) {
            // Use powers of 2 as they're exactly representable in binary fixed-point
            EXPECT_TRUE(0.5_fx == 0.5_fx);
            EXPECT_TRUE(0.25_fx == 0.25_fx);
            EXPECT_TRUE(0.125_fx == 0.125_fx);
            
            EXPECT_TRUE(0.5_fxd == 0.5_fxd);
            EXPECT_TRUE(0.0625_fxd == 0.0625_fxd);
            
            EXPECT_TRUE(0.5_fxr == 0.5_fxr);
            EXPECT_TRUE(0.03125_fxr == 0.03125_fxr);
        }

        TEST(FixedPointComparison, FixedPointGreater) {
            EXPECT_TRUE(2.5_fx > 1.5_fx);
            EXPECT_TRUE(0_fx > (-1.5_fx));
            EXPECT_TRUE((-1.5_fx) > (-2.5_fx));
            EXPECT_FALSE(1.5_fx > 1.5_fx);
            EXPECT_FALSE(1.5_fx > 2.5_fx);
            
            EXPECT_TRUE(2.5_fxd > 1.5_fxd);
            EXPECT_TRUE((-1.5_fxd) > (-2.5_fxd));
            
            EXPECT_TRUE(1.5_fxr > 0.5_fxr);
        }

        TEST(FixedPointComparison, FixedPointGreaterEdgeCase) {
            // Using powers of 2 for exact representation
            EXPECT_TRUE(0.25_fx > 0.125_fx);
            EXPECT_FALSE(0.125_fx > 0.125_fx);
            
            EXPECT_TRUE(0.0625_fxd > 0.03125_fxd);
            EXPECT_FALSE(0.03125_fxd > 0.03125_fxd);
            
            EXPECT_TRUE(0.0625_fxr > 0.03125_fxr);
            EXPECT_FALSE(0.03125_fxr > 0.03125_fxr);
        }

        TEST(FixedPointComparison, FixedPointLesser) {
            EXPECT_TRUE(1.5_fx < 2.5_fx);
            EXPECT_TRUE((-1.5_fx) < 0_fx);
            EXPECT_TRUE((-2.5_fx) < (-1.5_fx));
            EXPECT_FALSE(1.5_fx < 1.5_fx);
            EXPECT_FALSE(2.5_fx < 1.5_fx);
            
            EXPECT_TRUE(1.5_fxd < 2.5_fxd);
            EXPECT_TRUE((-2.5_fxd) < (-1.5_fxd));
            
            EXPECT_TRUE(0.5_fxr < 1.5_fxr);
        }

        TEST(FixedPointComparison, FixedPointLesserEdgeCase) {
            // Using powers of 2 for exact representation
            EXPECT_TRUE(0.125_fx < 0.25_fx);
            EXPECT_FALSE(0.125_fx < 0.125_fx);
            
            EXPECT_TRUE(0.03125_fxd < 0.0625_fxd);
            EXPECT_FALSE(0.03125_fxd < 0.03125_fxd);
            
            EXPECT_TRUE(0.03125_fxr < 0.0625_fxr);
            EXPECT_FALSE(0.03125_fxr < 0.03125_fxr);
        }

        TEST(FixedPointComparison, FixedPointEqualLesser) {
            EXPECT_TRUE(1.5_fx <= 1.5_fx);
            EXPECT_TRUE(1.5_fx <= 2.5_fx);
            EXPECT_TRUE((-2.5_fx) <= (-1.5_fx));
            EXPECT_FALSE(2.5_fx <= 1.5_fx);
            EXPECT_FALSE((-1.5_fx) <= (-2.5_fx));
            
            EXPECT_TRUE(1.5_fxd <= 1.5_fxd);
            EXPECT_TRUE(1.5_fxd <= 2.5_fxd);
            
            EXPECT_TRUE(0.5_fxr <= 1.5_fxr);
        }

        TEST(FixedPointComparison, FixedPointEqualLesserEdgeCase) {
            // Using powers of 2 for exact representation
            EXPECT_TRUE(0.125_fx <= 0.125_fx);
            EXPECT_TRUE(0.125_fx <= 0.25_fx);
            
            EXPECT_TRUE(0.03125_fxd <= 0.03125_fxd);
            EXPECT_TRUE(0.03125_fxd <= 0.0625_fxd);
            
            EXPECT_TRUE(0.03125_fxr <= 0.03125_fxr);
            EXPECT_TRUE(0.03125_fxr <= 0.0625_fxr);
        }

        TEST(FixedPointComparison, FixedPointEqualGreater) {
            EXPECT_TRUE(1.5_fx >= 1.5_fx);
            EXPECT_TRUE(2.5_fx >= 1.5_fx);
            EXPECT_TRUE((-1.5_fx) >= (-2.5_fx));
            EXPECT_FALSE(1.5_fx >= 2.5_fx);
            EXPECT_FALSE((-2.5_fx) >= (-1.5_fx));
            
            EXPECT_TRUE(2.5_fxd >= 1.5_fxd);
            EXPECT_TRUE((-1.5_fxd) >= (-2.5_fxd));
            
            EXPECT_TRUE(1.5_fxr >= 0.5_fxr);
        }

        TEST(FixedPointComparison, FixedPointEqualGreaterEdgeCase) {
            // Using powers of 2 for exact representation
            EXPECT_TRUE(0.125_fx >= 0.125_fx);
            EXPECT_TRUE(0.25_fx >= 0.125_fx);
            
            EXPECT_TRUE(0.03125_fxd >= 0.03125_fxd);
            EXPECT_TRUE(0.0625_fxd >= 0.03125_fxd);
            
            EXPECT_TRUE(0.03125_fxr >= 0.03125_fxr);
            EXPECT_TRUE(0.0625_fxr >= 0.03125_fxr);
        }

        TEST(FixedPointComparison, FixedPointNotEqual) {
            EXPECT_TRUE(1.5_fx != 2.5_fx);
            EXPECT_TRUE((-1.5_fx) != 1.5_fx);
            EXPECT_FALSE(1.5_fx != 1.5_fx);
            EXPECT_FALSE((-2.5_fx) != (-2.5_fx));
            
            EXPECT_TRUE(1.5_fxd != 2.5_fxd);
            EXPECT_FALSE((-2.5_fxd) != (-2.5_fxd));
            
            EXPECT_TRUE(0.5_fxr != 1.5_fxr);
            EXPECT_FALSE(3.0_fxr != 3.0_fxr);
        }

        TEST(FixedPointComparison, FixedPointNotEqualEdgeCase) {
            // Using powers of 2 for exact representation
            EXPECT_TRUE(0.125_fx != 0.25_fx);
            EXPECT_FALSE(0.125_fx != 0.125_fx);
            
            EXPECT_TRUE(0.03125_fxd != 0.0625_fxd);
            EXPECT_FALSE(0.03125_fxd != 0.03125_fxd);
            
            EXPECT_TRUE(0.03125_fxr != 0.0625_fxr);
            EXPECT_FALSE(0.03125_fxr != 0.03125_fxr);
        }
    #pragma endregion

    #pragma region Different type fixed point 
        TEST(FixedPointComparison, CrossFixedEqual) {
            EXPECT_TRUE(1.5_fx == 1.5_fxd);
            EXPECT_TRUE(1.5_fxd == 1.5_fx);
            EXPECT_TRUE(1.5_fx == 1.5_fxr);
            EXPECT_TRUE(1.5_fxr == 1.5_fx);
            EXPECT_TRUE(1.5_fxd == 1.5_fxr);
            EXPECT_TRUE(1.5_fxr == 1.5_fxd);
            EXPECT_FALSE(1.5_fx == 2.0_fxd);

            EXPECT_TRUE(0.125_fx == 0.125_fxd);
            EXPECT_TRUE(0.25_fxd == 0.25_fxr);
            EXPECT_TRUE(0.5_fxr == 0.5_fx);
            EXPECT_FALSE(0.25_fx == 0.125_fxd);
            EXPECT_FALSE(0.5_fxd == 0.25_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedGreater) {
            EXPECT_TRUE(2.0_fx > 1.5_fxd);
            EXPECT_TRUE(2.0_fxd > 1.5_fx);
            EXPECT_TRUE(2.0_fx > 1.5_fxr);
            EXPECT_TRUE(2.0_fxd > 1.5_fxr);
            EXPECT_FALSE(1.5_fx > 1.5_fxd);
            EXPECT_FALSE(1.5_fxr > 2.0_fx);

            EXPECT_TRUE(0.25_fx > 0.125_fxd);
            EXPECT_TRUE(0.25_fxd > 0.125_fxr);
            EXPECT_TRUE(0.25_fxr > 0.125_fx);
            EXPECT_FALSE(0.125_fx > 0.125_fxd);
            EXPECT_FALSE(0.125_fxd > 0.25_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedLesser) {
            EXPECT_TRUE(1.5_fx < 2.0_fxd);
            EXPECT_TRUE(1.5_fxd < 2.0_fx);
            EXPECT_TRUE(1.5_fxr < 2.0_fx);
            EXPECT_TRUE(1.5_fxr < 2.0_fxd);
            EXPECT_FALSE(1.5_fx < 1.5_fxd);

            EXPECT_TRUE(0.125_fx < 0.25_fxd);
            EXPECT_TRUE(0.125_fxd < 0.25_fxr);
            EXPECT_TRUE(0.125_fxr < 0.25_fx);
            EXPECT_FALSE(0.125_fx < 0.125_fxd);
            EXPECT_FALSE(0.25_fxd < 0.125_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedEqualLesser) {
            EXPECT_TRUE(1.5_fx <= 1.5_fxd);
            EXPECT_TRUE(1.5_fx <= 2.0_fxd);
            EXPECT_TRUE(1.5_fxd <= 1.5_fx);
            EXPECT_TRUE(1.5_fxr <= 2.0_fx);
            EXPECT_TRUE(1.5_fxd <= 1.5_fxr);
            EXPECT_TRUE(1.5_fxr <= 2.0_fxd);
            EXPECT_FALSE(2.0_fx <= 1.5_fxd);

            EXPECT_TRUE(0.125_fx <= 0.125_fxd);
            EXPECT_TRUE(0.125_fx <= 0.25_fxd);
            EXPECT_TRUE(0.125_fxd <= 0.125_fxr);
            EXPECT_TRUE(0.125_fxr <= 0.25_fx);
            EXPECT_FALSE(0.25_fx <= 0.125_fxd);
            EXPECT_FALSE(0.25_fxd <= 0.125_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedEqualGreater) {
            EXPECT_TRUE(1.5_fx >= 1.5_fxd);
            EXPECT_TRUE(2.0_fx >= 1.5_fxd);
            EXPECT_TRUE(1.5_fxd >= 1.5_fx);
            EXPECT_TRUE(1.5_fxd >= 1.5_fxr);
            EXPECT_FALSE(1.5_fx >= 2.0_fxd);
            EXPECT_FALSE(1.5_fxr >= 2.0_fx);

            EXPECT_TRUE(0.125_fx >= 0.125_fxd);
            EXPECT_TRUE(0.25_fx >= 0.125_fxd);
            EXPECT_TRUE(0.125_fxd >= 0.125_fxr);
            EXPECT_TRUE(0.25_fxr >= 0.125_fx);
            EXPECT_FALSE(0.125_fx >= 0.25_fxd);
            EXPECT_FALSE(0.125_fxd >= 0.25_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedNotEqual) {
            EXPECT_TRUE(1.5_fx != 2.0_fxd);
            EXPECT_TRUE(2.0_fxd != 1.5_fx);
            EXPECT_FALSE(1.5_fx != 1.5_fxd);
            EXPECT_FALSE(1.5_fxr != 1.5_fx);

            EXPECT_TRUE(0.125_fx != 0.25_fxd);
            EXPECT_TRUE(0.25_fxd != 0.125_fxr);
            EXPECT_TRUE(0.125_fxr != 0.25_fx);
            EXPECT_FALSE(0.125_fx != 0.125_fxd);
            EXPECT_FALSE(0.125_fxd != 0.125_fxr);
        }
        
        // Testing with negative values across types
        TEST(FixedPointComparison, CrossFixedNegativeValues) {
            EXPECT_TRUE((-1.5_fx) == (-1.5_fxd));
            EXPECT_TRUE((-2.0_fx) < (-1.5_fxd));
            EXPECT_TRUE((-1.5_fx) > (-2.0_fxd));
            EXPECT_TRUE((-1.5_fx) <= (-1.5_fxd));
            EXPECT_TRUE((-1.5_fx) != (-2.0_fxd));
        }
        
        // Testing with zero across types
        TEST(FixedPointComparison, CrossFixedWithZero) {
            EXPECT_TRUE(0_fx == 0_fxd);
            EXPECT_TRUE(0_fxr == 0_fx);
            EXPECT_TRUE(0_fxd == 0_fxr);
            EXPECT_TRUE(0_fx <= 0_fxd);
            EXPECT_TRUE(0_fxr >= 0_fx);
            EXPECT_FALSE(0_fx != 0_fxd);
        }

        TEST(FixedPointComparison, CrossFixedEqualEdgeCase) {
            // Create values that will overflow when compared across types
            // fx is int32 with 8 decimal bits
            // fxd is int64 with 11 decimal bits
            // fxr is uint16 with 15 decimal bits (smallest range)
            
            // Value too large for fxr but valid in fx/fxd
            fixed biggerThanRadianMax = 100.0_fx;  // Valid in fx, would overflow in fxr
            doubleFixed veryLargeVal = 1000000.0_fxd;  // Valid in fxd, would overflow in fx and fxr
            
            // Test cross-type comparisons where one would overflow in the other type
            EXPECT_FALSE(biggerThanRadianMax == 1.0_fxr);
            EXPECT_FALSE(veryLargeVal == 100.0_fx);
        }
        
        TEST(FixedPointComparison, CrossFixedGreaterEdgeCase) {
            // Values that would overflow if converted
            fixed biggerThanRadianMax = 100.0_fx;
            doubleFixed veryLargeVal = 1000000.0_fxd;
            
            // Test greater than with values that would overflow
            EXPECT_TRUE(biggerThanRadianMax > 1.0_fxr);
            EXPECT_TRUE(veryLargeVal > 100.0_fx);
            EXPECT_FALSE(1.0_fxr > biggerThanRadianMax);
        }
        
        TEST(FixedPointComparison, CrossFixedLesserEdgeCase) {
            // Values that would overflow if converted
            fixed biggerThanRadianMax = 100.0_fx;
            doubleFixed veryLargeVal = 1000000.0_fxd;
            
            // Test less than with values that would overflow
            EXPECT_TRUE(1.0_fxr < biggerThanRadianMax);
            EXPECT_TRUE(100.0_fx < veryLargeVal);
            EXPECT_FALSE(biggerThanRadianMax < 1.0_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedEqualLesserEdgeCase) {
            // Values that would overflow if converted
            fixed biggerThanRadianMax = 100.0_fx;
            doubleFixed veryLargeVal = 1000000.0_fxd;
            
            // Test less than or equal with values that would overflow
            EXPECT_TRUE(1.0_fxr <= biggerThanRadianMax);
            EXPECT_TRUE(100.0_fx <= veryLargeVal);
            EXPECT_FALSE(biggerThanRadianMax <= 1.0_fxr);
            
            // Test at the boundary
            EXPECT_TRUE(1.0_fx <= 1.0_fxd);
            EXPECT_TRUE(1.0_fxr <= 1.0_fx);
        }
        
        TEST(FixedPointComparison, CrossFixedEqualGreaterEdgeCase) {
            // Values that would overflow if converted
            fixed biggerThanRadianMax = 100.0_fx;
            doubleFixed veryLargeVal = 1000000.0_fxd;
            
            // Test greater than or equal with values that would overflow
            EXPECT_TRUE(biggerThanRadianMax >= 1.0_fxr);
            EXPECT_TRUE(veryLargeVal >= 100.0_fx);
            EXPECT_FALSE(1.0_fxr >= biggerThanRadianMax);
            
            // Test at the boundary
            EXPECT_TRUE(1.0_fxd >= 1.0_fx);
            EXPECT_TRUE(1.0_fx >= 1.0_fxr);
        }
        
        TEST(FixedPointComparison, CrossFixedNotEqualEdgeCase) {
            // Values that would overflow if converted
            fixed biggerThanRadianMax = 100.0_fx;
            doubleFixed veryLargeVal = 1000000.0_fxd;
            
            // Test not equal with values that would overflow
            EXPECT_TRUE(biggerThanRadianMax != 1.0_fxr);
            EXPECT_TRUE(veryLargeVal != 100.0_fx);
            EXPECT_FALSE(1.0_fx != 1.0_fxd);
        }

        TEST(FixedPointComparison, CrossFixedNegativeEqualEdgeCase) {
            // Create negative values that would underflow if converted to unsigned
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test cross-type equality with values that would underflow
            EXPECT_FALSE(negativeFixed == 0.5_fxr); // Negative vs unsigned
            EXPECT_FALSE(largeNegative == -10.0_fx); // Large negative vs small negative
            EXPECT_TRUE(negativeFixed == -10.0_fxd); // Same negative value across types
        }
        
        TEST(FixedPointComparison, CrossFixedNegativeGreaterEdgeCase) {
            // Values that would underflow if converted incorrectly
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test greater than with negative values and unsigned types
            EXPECT_FALSE(negativeFixed > 0.1_fxr); // Negative is never > unsigned
            EXPECT_FALSE(largeNegative > negativeFixed); // More negative isn't greater
            EXPECT_TRUE(negativeFixed > largeNegative); // Less negative is greater
            EXPECT_TRUE(0.1_fxr > negativeFixed); // Unsigned is always > negative
        }
        
        TEST(FixedPointComparison, CrossFixedNegativeLesserEdgeCase) {
            // Values that would underflow if converted incorrectly
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test less than with negative values and unsigned types
            EXPECT_TRUE(negativeFixed < 0.1_fxr); // Negative is always < unsigned
            EXPECT_TRUE(largeNegative < negativeFixed); // More negative is less
            EXPECT_FALSE(negativeFixed < largeNegative); // Less negative isn't less
            EXPECT_FALSE(0.1_fxr < negativeFixed); // Unsigned is never < negative
        }
        
        TEST(FixedPointComparison, CrossFixedNegativeEqualLesserEdgeCase) {
            // Values that would underflow if converted incorrectly
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test less than or equal with negative values
            EXPECT_TRUE(negativeFixed <= 0.1_fxr); // Negative is always <= unsigned
            EXPECT_TRUE(largeNegative <= negativeFixed); // More negative is less
            EXPECT_FALSE(negativeFixed <= largeNegative); // Less negative isn't less/equal
            EXPECT_FALSE(0.1_fxr <= negativeFixed); // Unsigned is never <= negative
            
            // Test equal values
            EXPECT_TRUE(negativeFixed <= -10.0_fxd); // Same value across types
        }
        
        TEST(FixedPointComparison, CrossFixedNegativeEqualGreaterEdgeCase) {
            // Values that would underflow if converted incorrectly
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test greater than or equal with negative values
            EXPECT_FALSE(negativeFixed >= 0.1_fxr); // Negative is never >= unsigned
            EXPECT_FALSE(largeNegative >= negativeFixed); // More negative isn't greater/equal
            EXPECT_TRUE(negativeFixed >= largeNegative); // Less negative is greater
            EXPECT_TRUE(0.1_fxr >= negativeFixed); // Unsigned is always >= negative
            
            // Test equal values
            EXPECT_TRUE(negativeFixed >= -10.0_fxd); // Same value across types
        }
        
        TEST(FixedPointComparison, CrossFixedNegativeNotEqualEdgeCase) {
            // Values that would underflow if converted incorrectly
            fixed negativeFixed = -10.0_fx;
            doubleFixed largeNegative = -1000000.0_fxd;
            
            // Test not equal with negative values
            EXPECT_TRUE(negativeFixed != 0.1_fxr); // Negative is never equal to unsigned
            EXPECT_TRUE(largeNegative != negativeFixed); // Different negative values
            EXPECT_FALSE(negativeFixed != -10.0_fxd); // Same value across types should be equal
        }
        
        TEST(FixedPointComparison, CrossFixedZeroWithNegative) {
            // Special cases with zero
            EXPECT_TRUE(0_fx == 0_fxr); // Zero is the same across all types
            EXPECT_TRUE(0_fxd > -1.0_fx); // Zero > negative
            EXPECT_TRUE(-1.0_fx < 0_fxr); // Negative < unsigned zero
            EXPECT_FALSE(-0.0_fx != 0_fxd); // Negative zero == zero
        }
    #pragma endregion
#pragma endregion

// Tests if simple operations between fixed point numbers work
#pragma region Math Operations
#pragma endregion


// Tests for casting to different numeric types
#pragma region Casting

    // No real need to test cross fixed point casting, since it is basically just calling a copy constructor

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

        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("000.125")), 0.125f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.015625")), 0.015625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.4140625")), 0.4140625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.90625")), .90625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("1.265625")), 1.265625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("1.734375")), 1.734375f);

        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("000.125")), 0.125f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.015625")), 0.015625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.4140625")), 0.4140625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("0.90625")), .90625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("1.265625")), 1.265625f);
        EXPECT_EQ((float)(FixedPoint<uint32, 31, false>("1.734375")), 1.734375f);   

        EXPECT_EQ((float)(radian("000.125")), 0.125f);
        EXPECT_EQ((float)(radian("0.015625")), 0.015625f);
        EXPECT_EQ((float)(radian("0.4140625")), 0.4140625f);
        EXPECT_EQ((float)(radian("0.90625")), .90625f);
        EXPECT_EQ((float)(radian("1.265625")), 1.265625f);
        EXPECT_EQ((float)(radian("1.734375")), 1.734375f);

        EXPECT_EQ((float)(radian("000.125")), 0.125f);
        EXPECT_EQ((float)(radian("0.015625")), 0.015625f);
        EXPECT_EQ((float)(radian("0.4140625")), 0.4140625f);
        EXPECT_EQ((float)(radian("0.90625")), .90625f);
        EXPECT_EQ((float)(radian("1.265625")), 1.265625f);
        EXPECT_EQ((float)(radian("1.734375")), 1.734375f);   
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

        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("000.125")), 0.125);
        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("0.015625")), 0.015625);
        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("0.4140625")), 0.4140625);
        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("0.90625")), .90625);
        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("1.265625")), 1.265625);
        EXPECT_EQ((double)(FixedPoint<uint32, 31, false>("1.734375")), 1.734375);

        EXPECT_EQ((double)(radian("000.125")), 0.125);
        EXPECT_EQ((double)(radian("0.015625")), 0.015625);
        EXPECT_EQ((double)(radian("0.4140625")), 0.4140625);
        EXPECT_EQ((double)(radian("0.90625")), .90625);
        EXPECT_EQ((double)(radian("1.265625")), 1.265625);
        EXPECT_EQ((double)(radian("1.734375")), 1.734375);
    }

    TEST(FixedPointConstructor, signedIntegerCast) {
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

        EXPECT_EQ((int8)(FixedPoint<uint32, 31, false>("1.1875")), 1);
        EXPECT_EQ((int16)(FixedPoint<uint32, 31, false>("0.1875")), 0);
        EXPECT_EQ((int32)(FixedPoint<uint32, 31, false>("1.544375")), 1);

        EXPECT_EQ((int8)(radian("1.1875")), 1);
        EXPECT_EQ((int8)(radian("1.52342875")), 1);
        EXPECT_EQ((int16)(radian("0.1875")), 0);
        EXPECT_EQ((int32)(radian("1.544375")), 1);
    }

    TEST(FixedPointConstructor, unsignedIntegerCast) {
        EXPECT_EQ((uint8)(43.1875_fx), 43);
        EXPECT_EQ((uint16)(473.1875_fx), 473);
        EXPECT_EQ((uint32)(473.1875_fx), 473);
        EXPECT_EQ((uint8)(43.1875_fxd), 43);
        EXPECT_EQ((uint16)(473.1875_fxd), 473);
        EXPECT_EQ((uint32)(473.1875_fxd), 473);
        EXPECT_EQ((uint8)(FixedPoint<uint32, 31, false>("1.1875")), 1);
        EXPECT_EQ((uint16)(FixedPoint<uint32, 31, false>("0.1875")), 0);
        EXPECT_EQ((uint32)(FixedPoint<uint32, 31, false>("1.1875")), 1);
        EXPECT_EQ((uint8)(radian("1.1875")), 1);
        EXPECT_EQ((uint16)(radian("0.1875")), 0);
        EXPECT_EQ((uint32)(radian("1.1875")), 1);
    }

    TEST(FixedPointConstructor, outofSignedBoundsCastFailure) {
        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(int8 warnPrevent = (int8)(128.5875_fx), ".*");
        EXPECT_EQ((int8)(127.5875_fx), int8(127));

        EXPECT_DEATH(int16 warnPrevent = (int16)(32768.8875_fx);, ".*");
        EXPECT_EQ((int16)(32767.5875_fx), int16(32767));

        EXPECT_DEATH(int8 warnPrevent = (int8)(128.8875_fxd);, ".*");
        EXPECT_EQ((int8)(127.5875_fxd), int8(127));

        EXPECT_DEATH(int16 warnPrevent = (int16)(32768.8875_fxd);, ".*");
        EXPECT_EQ((int16)(32767.5875_fxd), int16(32767));

        EXPECT_DEATH(int32 warnPrevent = (int32)(2147483648.88375_fxd);, ".*");
        EXPECT_EQ((int32)(2147483647.88375_fxd), int32(2147483647));

        EXPECT_DEATH(int8 warnPrevent = (int8)fixed("-129.5875"), ".*");
        EXPECT_EQ((int8)fixed("-128.5875"), int8(-128));

        EXPECT_DEATH(int16 warnPrevent = (int16)fixed("-32769.8875");, ".*");
        EXPECT_EQ((int16)fixed("-32768.5875"), int16(-32768));

        EXPECT_DEATH(int8 warnPrevent = (int8)doubleFixed("-129.8875");, ".*");
        EXPECT_EQ((int8)doubleFixed("-128.5875"), int8(-128));

        EXPECT_DEATH(int16 warnPrevent = (int16)doubleFixed("-32769.8875");, ".*");
        EXPECT_EQ((int16)doubleFixed("-32768.5875"), int16(-32768));

        EXPECT_DEATH(int32 warnPrevent = (int32)doubleFixed("-2147483649.88375");, ".*");
        EXPECT_EQ((int32)doubleFixed("-2147483648.88375"), int32(-2147483648));

        #endif
        EXPECT_TRUE(true);
    }

    TEST(FixedPointConstructor, outofUnsignedBoundsCastFailure) {
        #if DEBUGGING
        GTEST_FLAG_SET(death_test_style, "threadsafe");

        EXPECT_DEATH(uint8 warnPrevent = (uint8)(256.5875_fx), ".*");
        EXPECT_EQ((uint8)(255.5875_fx), uint8(255));

        EXPECT_DEATH(uint16 warnPrevent = (uint16)(65536.8875_fx);, ".*");
        EXPECT_EQ((uint16)(65535.5875_fx), uint16(65535));

        EXPECT_DEATH(uint8 warnPrevent = (uint8)(256.8875_fxd);, ".*");
        EXPECT_EQ((uint8)(255.5875_fxd), uint8(255));
        EXPECT_DEATH(uint16 warnPrevent = (uint16)(65536.8875_fxd);, ".*");
        EXPECT_EQ((uint16)(65535.5875_fxd), uint16(65535));

        EXPECT_DEATH(uint32 warnPrevent = (uint32)(4294967296.88375_fxd);, ".*");
        EXPECT_EQ((uint32)(4294967295.88375_fxd), uint32(4294967295));

        EXPECT_DEATH(uint8 warnPrevent = (uint8)fixed("-43.1875");, ".*");
        EXPECT_DEATH(uint16 warnPrevent = (uint16)fixed("-473.1875");, ".*");
        EXPECT_DEATH(uint32 warnPrevent = (uint32)fixed("-473.1875");, ".*");
        EXPECT_DEATH(uint8 warnPrevent = (uint8)doubleFixed("-43.1875");, ".*");
        EXPECT_DEATH(uint16 warnPrevent = (uint16)doubleFixed("-473.1875");, ".*");
        EXPECT_DEATH(uint32 warnPrevent = (uint32)doubleFixed("-473.1875");, ".*");
        #endif
        EXPECT_TRUE(true);
    }

#pragma endregion

#pragma region
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}