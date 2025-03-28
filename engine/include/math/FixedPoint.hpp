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
    // If Decimal Bit is 60 or higher a level of precision will be sacrificed in order to prevent overflow.
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
        {
            uint64 multIter = 1;
            for(size_t integerIter = integerPointStore.size() ; integerIter > 0 ; integerIter--) {
                integerPoint += integerPointStore[integerIter - 1] * multIter;
                multIter *= 10;
                // Hard limit for bits allocated to represent integers
                #if DEBUGGING
                    // A little bit bigger for negative due to two's compliment
                    if(negative) {
                        ASSERT(integerPoint <= (uint64(1) << ((sizeof(Base) * 8) - DecimalBits - 1)));
                    }
                    else {
                        ASSERT(integerPoint <= (uint64(1) << ((sizeof(Base) * 8) - DecimalBits - 1)) - 1);
                    }
                #endif
            }   
        }

        // More complex part processing decimal point
        // Done through long addition to prevent overflow
        uint64 decimalPoint = 0;

        {
            // Will sacrifice some precision to prevent overflow if needed
            constexpr bool overflowStopper = DecimalBits >= 60;
            constexpr uint64 mult = overflowStopper ? ((uint(1) << DecimalBits)/10) : (uint(1) << DecimalBits);

            // Adds up remainders through long addition done 
            for(size_t decimalIter = decimalPointStore.size() ; decimalIter > 0 ; decimalIter--) {
                size_t decIdx = decimalIter - 1;
                decimalPoint /= 10;
                if(decimalIter == 1 && decimalPoint % 10 >= 5) {
                    decimalPoint += 10;
                }
                decimalPoint += decimalPointStore[decIdx]  * mult;
            }
            
            if constexpr (!overflowStopper) {
                decimalPoint /= 10;
            }
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

    operator float () {
        if constexpr (FLOAT_IEEE754_REP) {
            //TODO
            return 0.0f;
        }
        else {
            return m_baseInt/((float)(1 << (1 + DecimalBits)));
        }
    }

    operator double () {
        if constexpr (FLOAT_IEEE754_REP) {
            //TODO
            return 0.0;
        }
        else {
            return m_baseInt/((double)(1 << (1 + DecimalBits)));
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