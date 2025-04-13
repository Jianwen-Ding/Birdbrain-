#ifndef FIXEDPOINT_HPP
#define FIXEDPOINT_HPP

#include "DEBUG.hpp"
#include "MathConsts.hpp"
#include "DetMathInt.hpp"

#include <bit>
#include <utility>
#include <cstddef>

// This class defines a fixed point number.
// This takes an integer class and allocates a number of bits to define what is past the decimal mark.
// Used to allow for decimal math without potential determinism problems of floating point math. 
// One way of thinking of the value of the class is {Store Base Number} / 2^(Decimal Bits).
// Any FixedPoint with FlowGuard enabled that will fail on overflow/underflow detected if debugging is also enabled.
// Overflow/underflow for signed numbers is undefined behavior so for those signed FixedPoints then guard is needed.
template <Int Base, uint8 DecimalBits, bool FlowGuard = true>
struct FixedPoint {

protected:
    template<Int, uint8 , bool>
    friend class FixedPoint;

    #pragma region Constexpr
        static constexpr bool m_isSigned = std::is_signed_v<Base>;
        // Equivalent to 2^DecimalBits
        static constexpr Base m_divisor = Base(1) << DecimalBits;

        static constexpr Base findMaxIntegerPoint() {
            if constexpr (DecimalBits == 0) {
                return std::numeric_limits<Base>::max();
            }
            else if constexpr (m_isSigned) {
                return (Base(1) << ((sizeof(Base) * 8) - DecimalBits - 1)) - 1;
            }
            else {
                return (Base(1) << ((sizeof(Base) * 8) - DecimalBits)) - 1;
            }
        }

        static constexpr Base findMinIntegerPoint() {
            if constexpr (m_isSigned) {
                return Base(-1) << ((sizeof(Base) * 8) - DecimalBits - 1);
            }
            else {
                return 0;
            }
        }

        static constexpr Base findMinDecimalPoint() {
            if constexpr (m_isSigned) {
                return (Base(-1) << DecimalBits);
            }
            else {
                return 0;
            }
        }

        static constexpr Base m_maxIntegerPoint = findMaxIntegerPoint();
        static constexpr Base m_minIntegerPoint = findMinIntegerPoint();
        static constexpr Base m_maxDecimalPoint = (Base(1) << DecimalBits) - 1;
        static constexpr Base m_minDecimalPoint = findMinDecimalPoint();

        static constexpr uint64 m_decimalPointMask = std::numeric_limits<uint64>::max() >> (64 - DecimalBits);
        static constexpr uint64 m_integerPointMask = ~m_decimalPointMask;
    #pragma endregion

    // How the FixedPoint stores its value
    // Represents the value of the FixedPoint divided by 2^(Decimal Bits)
    Base m_baseInt{ 0 };

    // >>> These static asserts makes sure templates given to FixedPoint do work together <<<
    #pragma region Asserts
        // Overflow for signed numbers is undefined behavior so FlowGuard needs to be within place for those numbers.
        static_assert(!m_isSigned || FlowGuard);

        // Constrains decimalBits to within size of Base
        static_assert(!m_isSigned || DecimalBits <= (sizeof(Base) * 8) - 1);
        static_assert(m_isSigned || DecimalBits <= (sizeof(Base) * 8));
    #pragma endregion

    // >>> Private helper functions made to assist public functions <<<
    #pragma region Helpers
        template <Int Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
        constexpr FixedPoint(Base2 base, bool direct) : FixedPoint(base) {
            if (direct) { 
                m_baseInt = base;
            } 
        }
    #pragma endregion
public:
    // >>> Constructors <<<
    #pragma region Constructors
        // Fixed point empty constructor leaves a value of 0
        constexpr FixedPoint() : m_baseInt( 0 ) {};

        template <Int Base2, uint8 DecimalBits2, bool FlowGuard2>
        constexpr FixedPoint(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& org) {
            // Makes sure negative signed FixedPoint does not get casted into an unsigned fixed point 
            ASSERT_LOG(std::is_signed_v<Base> || org.m_baseInt >= 0, "Negative fixed point casted into unsigned fixed number");
            if constexpr (std::is_same_v<Base2,Base> && DecimalBits == DecimalBits2) {
                m_baseInt = org.m_baseInt;
            }
            else {
                constexpr int16 decDiff = DecimalBits - DecimalBits2;
                if constexpr (decDiff > 0) {
                    // Makes sure that FixedPoint number does not cast into an overflow
                    ASSERT(!FlowGuard || std::cmp_greater_equal(std::numeric_limits<Base>::max() >> decDiff, org.m_baseInt));
                    // No need to check for negative underflow for unsigned numbers or positive
                    if constexpr (m_isSigned) {
                        constexpr Base decDiffDivisor = (1 << decDiff);
                        ASSERT(!FlowGuard || std::cmp_less_equal(std::numeric_limits<Base>::min() / decDiffDivisor,  org.m_baseInt)); 
                    }

                    m_baseInt = Base(org.m_baseInt) << decDiff;
                }
                else {
                    // Makes sure that FixedPoint number does not cast into an overflow
                    constexpr int16 reverseDecDiff = -decDiff;
                    // Makes sure that FixedPoint number does not cast into an overflow
                    ASSERT(!FlowGuard || std::cmp_less_equal(org.m_baseInt >> reverseDecDiff, std::numeric_limits<Base>::max()));
                    if constexpr (std::is_signed_v<Base2>) {
                        constexpr Base2 reverseDecDiffDivisor = (1 << reverseDecDiff);
                        // Check for negative underflow for signed numbers
                        ASSERT(!FlowGuard || std::cmp_greater_equal(org.m_baseInt / reverseDecDiffDivisor, std::numeric_limits<Base>::min())); 

                        m_baseInt = org.m_baseInt / reverseDecDiffDivisor;
                    }
                    else {
                        m_baseInt = org.m_baseInt >> reverseDecDiff;
                    }
                }
            }
        };

        template <Int Base2>
        constexpr FixedPoint(Base2 base) {
            // A negative number cannot be used to construct a unsigned fixed point
            ASSERT_LOG(m_isSigned || base >= 0, "Negative fixed point casted into unsigned int");
            // Makes sure that base number isn't too large to even be stored by m_baseInt 
            ASSERT_LOG(!FlowGuard || std::cmp_less_equal(base, std::numeric_limits<Base>::max() >> DecimalBits), "Fixed point number too high to be casted into int");
            ASSERT_LOG(!FlowGuard || m_isSigned ||  std::cmp_greater_equal(base, std::numeric_limits<Base>::min() >> DecimalBits), "Fixed point number too low to be casted into int");

            m_baseInt = base << DecimalBits;
        }

        // Used for FixedPoint string literals.
        // Will round number to closes applicable fixed point.
        // If debugger is on the constructor will fail if the number goes past the max of the fixed point.
        // It can at most be precise to 59 decimal bits due to overflow restrictions
        // Is capped to a digit length of 254, past that the constructor will stop reading
        constexpr FixedPoint(const char* str) {
            // Checks if reader has anything to read
            ASSERT_LOG(strlen(str) > 0, "Constructor has nothing to read off of");

            bool negative = false;
            bool hasReachedPoint = false;

            std::vector<uint8> integerPointStore{ };
            std::vector<uint8> decimalPointStore{ };
            // Will just ignore anything past 255 digits
            for(uint8 charIter = 0 ; charIter < 255  && charIter < std::strlen(str) ; charIter++) {
                // Checks for negative number
                if(charIter == 0) {
                    if(str[0] == '-') {
                        // Makes sure unsigned numbers are not marked with negative
                        ASSERT_LOG(m_isSigned, "Unsigned number marked as negative");
                        negative = true;
                        continue;
                    }
                }
        
                // Processes char into a integer
                uint8 val = -1;
                switch(str[charIter]) {
                    case '.':
                        // Checks for a second decimal point
                        ASSERT_LOG(!hasReachedPoint, "A second decimal point exists within " << str);
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
                        ASSERT_LOG(false, "Invalid character " << str[charIter] << " passed");
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
                uint64 limit;
                if (m_isSigned && negative) {
                    limit = uint64(-(m_minIntegerPoint + 1)) + 1;
                }
                else {
                    limit = m_maxIntegerPoint;
                }
                
                uint64 maxMult = DetMathInt::pow(10, DetMathInt::log(uint64(1) << 63, 10));
                uint8 maxMostSigDigit = limit/maxMult;
                #endif

                for(size_t integerIter = integerPointStore.size() ; integerIter > 0 ; integerIter--) {
                    integerPoint += integerPointStore[integerIter - 1] * multIter;
                    // Makes sure that integer doesn't exceed max size
                    ASSERT_LOG(!FlowGuard || !(multIter == maxMult && integerPointStore[integerIter - 1] > maxMostSigDigit) && integerPoint <= limit, str << " exceeds max size of " << limit);
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
                ASSERT_LOG(!FlowGuard || !atLimit || (decimalPoint < (uint64(1) << DecimalBits)), str << " exceeds max size after rounding");

                // Checks to see if any value at all is recorded in case of overflow at negative
                // (Two's compliment adds a value of space to the int point if decimal point is clear)
                ASSERT_LOG(!FlowGuard || !(atLimit && negative && decimalPoint > 0), str << " exceeds max size");
            }

            // Combines Integer point and decimal point
            uint64 final = (integerPoint << DecimalBits) + decimalPoint;
            // Adjusts final number based on whether the number was negative or not
            if constexpr (m_isSigned) {
                if(negative) {
                    m_baseInt =  -1 * Base(final);
                }
                else {
                    m_baseInt = Base(final);
                }
            }
            else {
                m_baseInt = Base(final);
            }
        }

    #pragma endregion

    // >>> Equality Operators (==, >, <, ect) <<< 
    #pragma region Equality
        template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
        constexpr bool operator ==(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
            if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                return m_baseInt == rhs.m_baseInt;
            }
            else if constexpr (DecimalBits2 > DecimalBits){
                constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                return Base2(m_baseInt) <= std::numeric_limits<Base2>::max() >> decDiff && Base2(m_baseInt) << decDiff == rhs.m_baseInt;
            }
            else {
                constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                return Base(rhs.m_baseInt) <= std::numeric_limits<Base>::max() >> decDiff && m_baseInt == Base(rhs.m_baseInt) << decDiff;
            }
        }
    #pragma endregion

    // >>> Basic Arithmetic Operators (+, -, /, ect) <<<
    #pragma region Arithmetic
        FixedPoint<Base,DecimalBits,FlowGuard> operator-() const {
            static_assert(m_isSigned);
            // Due to two's compliment if signed int is equal to min number *-1 cannot happen without overflow
            ASSERT(!FlowGuard || m_isSigned || m_baseInt != std::numeric_limits<Base>::min());

            return FixedPoint<Base,DecimalBits,FlowGuard>(m_baseInt * -1, true);
        }

        FixedPoint<Base,DecimalBits,FlowGuard> operator*(FixedPoint<Base,DecimalBits,FlowGuard> rhs) const {
            return (rhs.m_baseInt * m_baseInt) << DecimalBits;
        } 

        friend FixedPoint<Base,DecimalBits,FlowGuard> operator*(Base lhs, const FixedPoint<Base,DecimalBits,FlowGuard>& rhs) {
            return (lhs << DecimalBits) / rhs.m_baseInt;
        }

        FixedPoint<Base,DecimalBits,FlowGuard> operator/(FixedPoint<Base,DecimalBits,FlowGuard> rhs) const {
            return (rhs.m_baseInt * m_baseInt) << DecimalBits;
        } 

        friend FixedPoint<Base,DecimalBits,FlowGuard> operator/(Base lhs, const FixedPoint<Base,DecimalBits,FlowGuard>& rhs) {
            return (lhs << DecimalBits) / rhs.m_baseInt;
        }
    #pragma endregion

    // >>> Controls text representation of fixed point numbers <<<
    #pragma region String Representation
        // Streams the value of the Fixed Point number.
        // Only supports up to 19 decimal bits of precision due to overflow issues, will round to nearest 19 decimal bit representation if over. 
        // If rounding up would cause value to overflow. The decimal will round down instead.
        friend std::ostream& operator<<(std::ostream& out, const FixedPoint<Base, DecimalBits, FlowGuard>& obj)
        {
            return out << obj.toString().c_str();
        }

        using processInt = std::make_unsigned_t<Base>;
        static constexpr uint8 maxDecBitPrecision = 18;

        // Allows numbers to be floored to decimal bit that string can be represented int
        static inline constexpr uint8 floorRepDecBit() {
            if constexpr (DecimalBits > maxDecBitPrecision) {
                return maxDecBitPrecision;
            }
            else {
                return DecimalBits;
            }
        }

        // Reduces code duplication of toString.
        // Stop overflow checks to see if integer point is at limit and thus decimal point should not ever round up to 1.
        inline std::string unsignedToString(processInt num, bool stopOverflow) {      
            // Adds integer point    
            std::string retString = std::to_string(num >> DecimalBits);    
            processInt intPoint = num >> DecimalBits;
            // Adds for decimal point and marker
            static constexpr uint8 repDecimalBits = floorRepDecBit();
            constexpr int16 decDiff = DecimalBits - maxDecBitPrecision;

            processInt repDecimalNum;
            if constexpr (decDiff > 0) {
                repDecimalNum = (num & m_decimalPointMask) >> (decDiff - 1);
                LOG("Resulting number: " << repDecimalNum);
                // Rounding logic
                if (repDecimalNum & 1) {
                    repDecimalNum >>= 1;
                    // Prevents rounding up if it would cause overflow
                    if(!stopOverflow || repDecimalNum < (uint64(1) << repDecimalBits) - 1) {
                        repDecimalNum += 1;
                    }
                }
                else {
                    repDecimalNum >>= 1;
                }
                LOG("Resulting number: " << repDecimalNum);
            }
            else {
                repDecimalNum = (num & m_decimalPointMask);
            }

            uint64 decimalNum = repDecimalNum * DetMathInt::pow(int64(5), repDecimalBits);
            if(decimalNum != 0) {
                uint64 cutNum;
                bool foundCut = false;
                uint8 fullDecLength = 0;
                LOG("Resulting Dec Num: " << decimalNum);
                while(decimalNum > 0) {
                    if(!foundCut && decimalNum % 10 != 0) {
                        cutNum = decimalNum;
                        foundCut = true;
                    }
                    decimalNum /= 10;
                    fullDecLength++;
                }
                ASSERT_LOG(foundCut, "Despite decimalNum not being zero, a non 0 decimal could not be found");

                retString += '.';
                LOG(retString);
                retString.resize(retString.size() + repDecimalBits - fullDecLength, '0');
                LOG(retString);
                retString.append(std::to_string(cutNum));
                LOG(retString);
            }
            return retString;
        }
        
        // Converts value of fixed point into a string.
        // Only supports up to 19 bits of decimal precision due to overflow issues, will round to nearest 19 decimal bit representation if over.
        // If rounding up would cause an overflow, the decimals will be rounded down instead.
        std::string toString() {
            if (DecimalBits == 0) {
                return std::to_string(m_baseInt);
            }
            else if constexpr (m_isSigned) {
                // Needs to account for a possibility of a negative number
                // As a result some code duplication may occur
                bool negative = m_baseInt < 0;
                processInt processNum;
                if(negative) {
                    // Prevents overflow by going to negative number
                    processNum = processInt(-(m_baseInt + 1)) + 1;
                    return '-' + unsignedToString(processNum, false);
                }
                else {
                    processNum = processInt(m_baseInt);
                    return unsignedToString(processNum, ((m_baseInt & m_integerPointMask) >> DecimalBits) >= m_maxIntegerPoint);
                }
            }
            else {
                return unsignedToString(m_baseInt, ((m_baseInt & m_integerPointMask) >> DecimalBits) >= m_maxIntegerPoint);
            }
        }
    #pragma endregion

    // >>> Casts to other numeric types <<<
    #pragma region Casts
        // Casts a FixedPoint number into another FixedPoint number
        template <Int Base2, uint8 DecimalBits2, bool FlowGuard2>
        operator FixedPoint<Base2, DecimalBits2, FlowGuard2> () const {
            return FixedPoint<Base2, DecimalBits2, FlowGuard2>(*this);
        }

        // Truncates FixedPoint value to integer.
        template <Int Base2>
        operator Base2 () const {
            // Makes sure a negative number is not casted into an unsigned number
            ASSERT(std::is_signed_v<Base2> || m_baseInt >= 0);
            // Makes sure number isn't too large to be casted into specified int, flow guard does not apply since int is being casted into
            ASSERT(std::cmp_less_equal(m_baseInt >> DecimalBits, std::numeric_limits<Base2>::max()));
            if(m_isSigned) {
                ASSERT(std::cmp_greater_equal(m_baseInt / m_divisor, std::numeric_limits<Base2>::min()));
            }

            if constexpr (m_isSigned) {
                // Can't bit shift for potential negative numbers because bit shift is stupid and rounds towards negative infinity xd
                return Base2(m_baseInt / m_divisor);
            }
            else {
                return Base2(m_baseInt >> DecimalBits);
            }
        }

        // Packs base int into a unsigned int that can be bitcasted into a floating point number in IEEE 754 format
        // We don't need to be as cautious in terms of accounting for all possible variations of this, we only need to ensure that it works for float and double
        template <FloatingPoint retFloat, UnsignedInt uFinalInt, UnsignedInt uCorInt, size_t mantissaSize, typename = std::enable_if_t<sizeof(retFloat) == sizeof(uFinalInt) && sizeof(uFinalInt) <= sizeof(uCorInt)>>
        inline retFloat processFloatBits() const {
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

        // Approximates FixedPoint to float representation
        // Main use will be for plugging from Logic to Rendering systems
        // DO NOT USE WITHIN LOGIC, THIS IS NON-DETERMINISTIC, THIS RISKS COMPROMISING THE DETERMINISM OF THE LOGIC
        operator float () const {
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
                return m_baseInt/((float)(m_divisor));
            }
        }

        // Approximates FixedPoint to double representation
        // Main use will be for plugging from Logic to Rendering systems
        // DO NOT USE WITHIN LOGIC, THIS IS NON-DETERMINISTIC, THIS RISKS COMPROMISING THE DETERMINISM OF THE LOGIC
        operator double () const {
            if constexpr (FLOAT_IEEE754_REP) {
                using corUInt = std::make_unsigned<Base>::type;
                if constexpr (sizeof(corUInt) >= sizeof(uint64)) {
                    return processFloatBits<double, uint64, corUInt, 52>();
                }
                else {
                    return processFloatBits<double, uint64, uint64, 52>();
                }
            }
            else {
                return m_baseInt/((double)(m_divisor));
            }
        }
    #pragma endregion

    // Fetches the underlying integer that the fixed point stores it's data within.
    Base getBase() {
        return m_baseInt;
    }
};

// Defines typical configurations that fixed point numbers come in
typedef FixedPoint<int32, 8, true> fixed;
typedef FixedPoint<int64, 11, true> doubleFixed;
typedef FixedPoint<uint32, 31, false> radian;

// Converts text into a fixed point radian.
// Will round number to closes applicable fixed point.
// Will fail if the number goes past the max of the fixed point.
// It can at most be precise to 59 decimal bits due to overflow restrictions.
// Is capped to a digit length of 254, past that the constructor will stop reading.
constexpr radian operator"" _fxr(const char* str) {
    return radian(str);
}

// Converts text into a double precision fixed point number.
// Will round number to closes applicable fixed point.
// Will fail if the number goes past the max of the fixed point.
// It can at most be precise to 59 decimal bits due to overflow restrictions.
// Is capped to a digit length of 254, past that the constructor will stop reading.
constexpr doubleFixed operator"" _fxd(const char* str) {
    return doubleFixed(str);
}

// Converts text into a single precision fixed point number.
// Will round number to closes applicable fixed point.
// Will fail if the number goes past the max of the fixed point.
// It can at most be precise to 59 decimal bits due to overflow restrictions.
// Is capped to a digit length of 254, past that the constructor will stop reading.
constexpr fixed operator"" _fx(const char* str) {
    return fixed(str);
}


#endif