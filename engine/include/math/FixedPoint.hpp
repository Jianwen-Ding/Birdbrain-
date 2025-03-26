#ifndef FIXEDPOINT_HPP
#define FIXEDPOINT_HPP

#include "DEBUG.hpp"
#include "MathConsts.hpp"

#include <bit>
#include <cstddef>

// This class defines a fixed point number,
// this takes an integer class and allocates a number of bits to define what is below the decimal.
// Used to allow for decimal math without potential determinism problems of floating point math. 
template <SignedInt Base, uint8 DecimalBits, typename = std::enable_if_t<DecimalBits < (sizeof(Base) * 8) - 1>>
struct FixedPoint {

private:
    static constexpr uint64 divisor = uint64(1) << (1 + DecimalBits);
    Base baseInt{ 0 };

    // >>> Private helper functions made to assist public functions <<<
    #pragma region Helpers
        template <SignedInt Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
        constexpr FixedPoint(Base2 base, bool direct) : FixedPoint(base){
            if (direct) { 
                baseInt = base;
            } 
        }

        constexpr bool HasLength(const char* str) {
            return strlen(str) != 0;
        }
    #pragma endregion

public:
    // >>> Constructors <<<
    #pragma region Constructors
    constexpr FixedPoint() : baseInt( 0 ) {};

    template <SignedInt Base2, uint8 DecimalBits2>
    constexpr FixedPoint(FixedPoint<Base2, DecimalBits2>& org) {
        // 
        if constexpr ((!std::is_same_v<Base, Base2> || (DecimalBits != DecimalBits2))) {
            int decDiff = DecimalBits - DecimalBits2;
        }
        // Directly copies base int if templates are the same
        else {
            baseInt = org.baseInt;
        }
    };

    template <SignedInt Base2, uint8 DecimalBits2>
    constexpr FixedPoint(FixedPoint<Base, DecimalBits>&& org) {
        //
        if constexpr ((!std::is_same_v<Base, Base2> || (DecimalBits != DecimalBits2))) {

        }
        // Directly copies base int if templates are the same
        else {
            baseInt.baseInt;
        }
    };

    template <SignedInt Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
    constexpr FixedPoint(Base2 base) {
        baseInt = base >> DecimalBits;
    }

    constexpr FixedPoint(const char* str) {
        // Checks if reader has anything to read
        ASSERT(strlen(str));
    
        // Scans char for integer and decimal part of the number
        bool negative = false;
        std::vector<uint8> integerPointStore{ };
        std::vector<uint8> decimalPointStore{ };
        bool hasReachedPoint = false;
        for(uint charIter = 0 ; charIter < 255  && charIter < std::strlen(str) ; charIter++) {
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
                    if(hasReachedPoint) {
                        ASSERT(false);
                    }
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
    
        // Processes integer and decimal part of the number
        uint64 integerPoint = 0;
        uint64 decimalPoint = 0;
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

        multIter = 1 << DecimalBits;
        for(size_t decimalIter = 0 ; decimalIter < decimalPointStore.size() && multIter > 0 ; decimalIter++) {
            multIter /= 10;
            decimalPoint += decimalPointStore[decimalIter] * multIter;
        }
        
        uint64 final = (integerPoint << DecimalBits) + decimalPoint;

        if(negative) {
            baseInt =  -1 * Base(final);
        }
        else {
            baseInt = Base(final);
        }
    }

    #pragma endregion

    // >>> Basic Arithmetic Functions (+, -, /, ect) <<<
    #pragma region Arithmetic

    FixedPoint<Base,DecimalBits> operator/(FixedPoint<Base,DecimalBits> rhs) {
        return (rhs.baseInt * baseInt) << DecimalBits;
    } 

    friend FixedPoint<Base,DecimalBits> operator/(Base lhs, const FixedPoint<Base,DecimalBits>& rhs) {
        return (lhs << DecimalBits) / rhs.baseInt;
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
            return baseInt/((float)(1 << (1 + DecimalBits)));
        }
    }

    operator double () {
        if constexpr (FLOAT_IEEE754_REP) {
            //TODO
            return 0.0;
        }
        else {
            return baseInt/((double)(1 << (1 + DecimalBits)));
        }
    }
    #pragma endregion

    // Fetches the underlying integer that the fixed point stores it's data within.
    Base getBase() {
        return baseInt;
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