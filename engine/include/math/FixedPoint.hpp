#ifndef FIXEDPOINT_HPP
#define FIXEDPOINT_HPP

#include "DEBUG.hpp"
#include "MathConsts.hpp"
#include "DetMathInt.hpp"

#include <bit>
#include <cstddef>

// This class defines a fixed point number,
// this takes an integer class and allocates a number of bits to define what is below the decimal.
// Used to allow for decimal math without potential determinism problems of floating point math. 
template <SignedInt Base, uint8 DecimalBits, typename = std::enable_if_t<DecimalBits <= (sizeof(Base) * 8) - 1>>
struct FixedPoint {

private:
    Base m_baseInt{ 0 };

    // >>> Private helper functions made to assist public functions <<<
    #pragma region Helpers
        template <SignedInt Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
        constexpr FixedPoint(Base2 base, bool direct) : FixedPoint(base){
            if (direct) { 
                m_baseInt = base;
            } 
        }

        constexpr bool HasLength(const char* str) {
            return strlen(str) != 0;
        }
    #pragma endregion

public:
    // >>> Constructors <<<
    #pragma region Constructors
    constexpr FixedPoint() : m_baseInt( 0 ) {};

    template <SignedInt Base2, uint8 DecimalBits2>
    constexpr FixedPoint(FixedPoint<Base2, DecimalBits2>& org) {
        // 
        if constexpr ((!std::is_same_v<Base, Base2> || (DecimalBits != DecimalBits2))) {
            int decDiff = DecimalBits - DecimalBits2;
        }
        // Directly copies base int if templates are the same
        else {
            m_baseInt = org.m_baseInt;
        }
    };

    template <SignedInt Base2, uint8 DecimalBits2>
    constexpr FixedPoint(FixedPoint<Base, DecimalBits>&& org) {
        //
        if constexpr ((!std::is_same_v<Base, Base2> || (DecimalBits != DecimalBits2))) {

        }
        // Directly copies base int if templates are the same
        else {
            m_baseInt = org.m_baseInt;
        }
    };

    template <SignedInt Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
    constexpr FixedPoint(Base2 base) {
        m_baseInt = base >> DecimalBits;
    }

    // Used for FixedPoint string literals.
    // Will fail on too high a number.
    // It can at most be precise to 59 decimal bits due to overflow restrictions
    constexpr FixedPoint(const char* str) {
        // Checks if reader has anything to read
        ASSERT(strlen(str) >= 0);

        bool negative = false;
        bool hasReachedPoint = false;

        std::vector<uint8> integerPointStore{ };
        std::vector<uint8> decimalPointStore{ };
        for(uint8 charIter = 0 ; charIter < 255  && charIter < std::strlen(str) ; charIter++) {
            // Checks for negative number
            if(charIter == 0) {
                if(str[0] == '-') {
                    negative = true;
                    continue;
                }
            }
    
            // Processes char into a integer
            uint8 val = -1;
            switch(str[charIter]) {
                case '.':
                    // Checks for a second decimal point
                    ASSERT(!hasReachedPoint);
                    hasReachedPoint = true;
                    continue;
                case '0':
                    // Prevents extraneous 0s from being stored
                    if(!hasReachedPoint && integerPointStore.size() == 0) {
                        continue;
                    }
                    else {
                        val = 0;
                    }
                    break;
                case '1':
                    val = 1;
                    break;
                case '2':
                    val = 2;
                    break;
                case '3':
                    val = 3;
                    break;
                case '4':
                    val = 4;
                    break;
                case '5':
                    val = 5;
                    break;
                case '6':
                    val = 6;
                    break;
                case '7':
                    val = 7;
                    break;
                case '8':
                    val = 8;
                    break;
                case '9':
                    val = 9;
                    break;
                default:
                    // Invalid char
                    ASSERT(false);
            }
            if(hasReachedPoint) {
                decimalPointStore.push_back(val);
            }
            else {
                integerPointStore.push_back(val);
            }
    
    
        }
    
        // Processes integer part of number
        uint64 integerPoint = 0;
        #if DEBUGGING 
        bool atLimit = false;
        #endif 
        {
            uint64 multIter = 1;
            
            #if DEBUGGING
            // Hard limit for bits allocated to represent integers
            // A little bit bigger for negative due to two's compliment
            uint64 limit;
            if(negative) {
                limit = (uint64(1) << ((sizeof(Base) * 8) - DecimalBits - 1));
            }
            else {
                limit = (uint64(1) << ((sizeof(Base) * 8) - DecimalBits - 1)) - 1;
            }
            uint64 maxMult = DetMathInt::pow(10, DetMathInt::log(uint64(1) << 63, 10));
            uint8 maxMostSigDigit = limit/maxMult;
            #endif

            for(size_t integerIter = integerPointStore.size() ; integerIter > 0 ; integerIter--) {
                integerPoint += integerPointStore[integerIter - 1] * multIter;
                ASSERT(!(multIter == maxMult && integerPointStore[integerIter - 1] > maxMostSigDigit) && integerPoint <= limit);
                multIter *= 10;
            }   

            #if DEBUGGING
            if(integerPoint == limit) {
                atLimit = true;
            }
            #endif
        }

        // More complex part processing decimal point
        // Done through long addition to prevent overflow
        uint64 decimalPoint = 0;

        {
            // Will sacrifice some precision to prevent overflow if needed
            constexpr bool overflowStopper = DecimalBits > 59;
            
            const uint64 mult = (uint64(1) << DecimalBits);

            // Adds up remainders through long addition done 
            for(size_t decimalIter = decimalPointStore.size() ; decimalIter > 0 ; decimalIter--) {
                size_t decIdx = decimalIter - 1;
                decimalPoint /= 10;
                if constexpr (overflowStopper) {
                    decimalPoint += decimalPointStore[decIdx]  * (uint64(1) << 59);
                }
                else {
                    decimalPoint += decimalPointStore[decIdx]  * mult;
                }

                // Rounding logic
                if(decimalIter == 1 && decimalPoint % 10 >= 5) {
                    decimalPoint += 10;
                }
            }
            
            decimalPoint /= 10;
            if constexpr (overflowStopper) {
                decimalPoint <<= (DecimalBits - 59);
            }

            // Checks to make sure that rounding won't result in overflow
            ASSERT(!atLimit || (decimalPoint < (uint64(1) << DecimalBits)));

            // Checks to see if any value at all is recorded in case of overflow at negative
            // (Two's compliment adds a value of space to the int point if decimal point is clear)
            ASSERT(!(atLimit && negative && decimalPoint > 0));
        }

        uint64 final = (integerPoint << DecimalBits) + decimalPoint;

        if(negative) {
            m_baseInt =  -1 * Base(final);
        }
        else {
            m_baseInt = Base(final);
        }
    }

    #pragma endregion

    // >>> Basic Arithmetic Functions (+, -, /, ect) <<<
    #pragma region Arithmetic
    FixedPoint<Base,DecimalBits> operator-() {
        return FixedPoint<Base,DecimalBits>(m_baseInt * -1, true);
    }

    FixedPoint<Base,DecimalBits> operator/(FixedPoint<Base,DecimalBits> rhs) {
        return (rhs.m_baseInt * m_baseInt) << DecimalBits;
    } 

    friend FixedPoint<Base,DecimalBits> operator/(Base lhs, const FixedPoint<Base,DecimalBits>& rhs) {
        return (lhs << DecimalBits) / rhs.m_baseInt;
    }

    #pragma endregion

    // >>> Casts to other numeric types <<<
    #pragma region Casts
    
    operator Base () {
        return 0;
    }

    // Packs base int into a unsigned int that can be bitcasted into a floating point number in IEEE 754 format
    // We don't need to be as cautious in terms of accounting for all possible variations of this, we only need to ensure that it works for float and double
    template <FloatingPoint retFloat, UnsignedInt uFinalInt, UnsignedInt uCorInt, size_t mantissaSize, typename = std::enable_if_t<sizeof(retFloat) == sizeof(uFinalInt) && sizeof(uFinalInt) <= sizeof(uCorInt)>>
    inline retFloat processFloatBits() {
        // Defining Constexpr variables
        // 1 bits for everything in mantissa bit space
        constexpr uCorInt mantissaOp = std::numeric_limits<uCorInt>::max() >> ((sizeof(uCorInt) * 8) - mantissaSize);
        // The value that represents 0 for the specific floating point
        constexpr uFinalInt zeroPoint = (uFinalInt(1) << ((sizeof(uFinalInt) * 8) - mantissaSize - 2)) - 1;

        // Handles 0 edge case
        if(m_baseInt == 0) {
            return retFloat(0);
        }

        // Takes away sign from number in order to allow for bit casting operations
        bool negative = m_baseInt < 0;
        uCorInt bits = uCorInt(negative ? m_baseInt * -1 : m_baseInt);
        uint8 width = std::bit_width(bits);

        // Shifts the mantissa into place
        int8 bitDiff = width - mantissaSize;
        if(bitDiff > 0) {
            bits >>= bitDiff - 1;
        }
        else {
            bits <<= -bitDiff + 1;
        }
        bits &= mantissaOp;

        // Shifts exponent into place
        bits |= ((width - DecimalBits - 1) + zeroPoint) << mantissaSize;
        // Shifts sign into place
        if(negative) {
            bits |= uFinalInt(1) << ((sizeof(uFinalInt) * 8) - 1);
        }

        return std::bit_cast<retFloat>(uFinalInt(bits));
    }

    operator float () {
        if constexpr (FLOAT_IEEE754_REP) {
            using corUInt = std::make_unsigned<Base>::type;
            if constexpr (sizeof(corUInt) >= sizeof(uint32)) {
                return processFloatBits<float, uint32, corUInt, 23>();
            }
            else {
                return processFloatBits<float, uint32, uint32, 23>();
            }
        }
        else {
            return m_baseInt/((float)(1 << DecimalBits));
        }
    }

    operator double () {
        if constexpr (FLOAT_IEEE754_REP) {
            using corUInt = std::make_unsigned<Base>::type;
            if constexpr (sizeof(corUInt) >= sizeof(uint64)) {
                return processFloatBits<float, uint64, corUInt, 52>();
            }
            else {
                return processFloatBits<float, uint64, uint64, 52>();
            }
        }
        else {
            return m_baseInt/((double)(1 << DecimalBits));
        }
    }
    #pragma endregion

    // Fetches the underlying integer that the fixed point stores it's data within.
    Base getBase() {
        return m_baseInt;
    }
};

typedef FixedPoint<int, 8> fixed;
typedef FixedPoint<int64, 11> doubleFixed;

constexpr doubleFixed operator"" _fxd(const char* str) {
    return doubleFixed(str);
}

constexpr fixed operator"" _fx(const char* str) {
    return fixed(str);
}


#endif