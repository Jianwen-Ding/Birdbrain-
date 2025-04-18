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
    // How the FixedPoint stores its value
    // Represents the value of the FixedPoint divided by 2^(Decimal Bits)
    Base m_baseInt{ 0 };

    template<Int, uint8 , bool>
    friend class FixedPoint;

    #pragma region Constexpr
        // A bunch of helper variables calculated at compile time 

        static constexpr bool m_isSigned = std::is_signed_v<Base>;
        // Equivalent to 2^DecimalBits
        static constexpr Base m_divisor = Base(1) << DecimalBits;
        static constexpr bool m_decimalBitsMaxed = (m_isSigned && DecimalBits == sizeof(Base) * 8 - 1) || (!m_isSigned && DecimalBits == sizeof(Base) * 8);

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
            if constexpr (DecimalBits == 0) {
                return std::numeric_limits<Base>::min();
            }
            else if constexpr (m_isSigned) {
                return (Base(-1) << ((sizeof(Base) * 8) - DecimalBits - 1));
            }
            else {
                return 0;
            }
        }

        static constexpr Base findMaxDecimalPoint() {
            if constexpr (m_decimalBitsMaxed) {
                return std::numeric_limits<Base>::max();
            }
            else {
                return (Base(1) << DecimalBits) - 1;
            }
        }

        static constexpr Base findMinDecimalPoint() {
            if constexpr (m_isSigned) {
                if constexpr (DecimalBits == sizeof(Base) * 8 - 1) {
                    return std::numeric_limits<Base>::min();
                }
                else {
                    return (Base(-1) << DecimalBits) + 1;
                }
            }
            else {
                return 0;
            }
        }

        static constexpr Base m_maxIntegerPoint = findMaxIntegerPoint();
        static constexpr Base m_minIntegerPoint = findMinIntegerPoint();
        static constexpr Base m_maxDecimalPoint = findMaxDecimalPoint();
        static constexpr Base m_minDecimalPoint = findMinDecimalPoint();

        static constexpr uint64 m_decimalPointMask = std::numeric_limits<uint64>::max() >> (64 - DecimalBits);
        static constexpr uint64 m_integerPointMask = ~m_decimalPointMask;

        using processInt = std::make_unsigned_t<Base>;
        static constexpr uint8 m_maxDecBitPrecision = 18;
    #pragma endregion

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
        // Allows direct constructor to call in integer constructor logic without duplication logic
        template <Int Base2>
        constexpr void integerInit(Base2 base) {
            // A negative number cannot be used to construct a unsigned fixed point
            ASSERT_LOG(m_isSigned || base >= 0, "Negative integer casted into unsigned fixed point");
            // Makes sure that base number isn't too large to even be stored by m_baseInt 
            ASSERT_LOG(!FlowGuard || std::cmp_less_equal(base, std::numeric_limits<Base>::max() >> DecimalBits), "Integer [" << base << "] higher than integer limit [" << (std::numeric_limits<Base>::max() >> DecimalBits) << "] and thus can't be used to construct fixed point number");
            ASSERT_LOG(!FlowGuard || !m_isSigned ||  std::cmp_greater_equal(base, std::numeric_limits<Base>::min() >> DecimalBits), "Integer [" << base << "] lower than integer minimum [" << (std::numeric_limits<Base>::max() >> DecimalBits) << "] and thus can't be used to construct fixed point number");

            m_baseInt = base << DecimalBits;
        }

        template <Int Base2, typename = std::enable_if_t<sizeof(Base) >= sizeof(Base2)>>
        constexpr FixedPoint(Base2 base, bool direct){
            if (direct) { 
                m_baseInt = base;
            }
            else {
                integerInit(base);
            }
        }

        // Allows decimal bit to be clamped to a certain max number, used in cases where precision needs to be sacrificed to prevent overflow.
        template <uint8 maxNum>
        static inline constexpr uint8 floorRepDecBit() {
            if constexpr (DecimalBits > maxNum) {
                return maxNum;
            }
            else {
                return DecimalBits;
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

        // Reduces code duplication of toString.
        // Stop overflow checks to see if integer point is at limit and thus decimal point should not ever round up to 1.
        inline std::string unsignedToString(processInt num, bool stopOverflow) const {      
            // Adds integer point    
            std::string retString; 
            if constexpr (m_decimalBitsMaxed) {
                retString = "0";
            }
            else {
                retString = std::to_string(num >> DecimalBits);    
            }
            // Adds for decimal point and marker
            static constexpr uint8 repDecimalBits = floorRepDecBit<m_maxDecBitPrecision>();
            constexpr int16 decDiff = DecimalBits - m_maxDecBitPrecision;

            processInt repDecimalNum;
            if constexpr (decDiff > 0) {
                repDecimalNum = (num & m_decimalPointMask) >> (decDiff - 1);
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
            }
            else {
                repDecimalNum = (num & m_decimalPointMask);
            }

            uint64 decimalNum = repDecimalNum * DetMathInt::pow(int64(5), repDecimalBits);
            if(decimalNum != 0) {
                uint64 cutNum;
                bool foundCut = false;
                uint8 fullDecLength = 0;
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
                retString.resize(retString.size() + repDecimalBits - fullDecLength, '0');
                retString.append(std::to_string(cutNum));
            }
            return retString;
        }
        
        // Allows for code deduplication between integer comparison and fixed point comparison
        #pragma region Comparison
            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool equals(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base>) {
                        return m_baseInt == num;
                    }
                    else {
                        return std::cmp_equal(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater_equal(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && std::cmp_less_equal(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && std::cmp_equal(Base2(m_baseInt) << decDiff, num);
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater_equal(num, std::numeric_limits<Base>::min() >> decDiff) && std::cmp_less_equal(num, std::numeric_limits<Base>::max() >> decDiff) && std::cmp_equal(m_baseInt, Base(num) << decDiff);
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool greater(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt > num;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_greater(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && (std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_greater(Base2(m_baseInt) << decDiff, num));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less(num, std::numeric_limits<Base>::max() >> decDiff) && (std::cmp_less(num, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater(m_baseInt, Base(num) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool lesser(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt < num;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_less(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && (std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_less(Base2(m_baseInt) << decDiff, num));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater(num, std::numeric_limits<Base>::min() >> decDiff) && (std::cmp_greater(num, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_less(m_baseInt, Base(num) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool greaterEqual(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt >= num;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_greater_equal(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater_equal(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && (std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_greater_equal(Base2(m_baseInt) << decDiff, num));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less_equal(num, std::numeric_limits<Base>::max() >> decDiff) && (std::cmp_less(num, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater_equal(m_baseInt, Base(num) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool lesserEqual(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt <= num;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_less_equal(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less_equal(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && (std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_less_equal(Base2(m_baseInt) << decDiff, num));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater_equal(num, std::numeric_limits<Base>::min() >> decDiff) && (std::cmp_greater(num, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_less_equal(m_baseInt, Base(num) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2>
            constexpr inline bool notEqual(const Base2 num) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base>) {
                        return m_baseInt != num;
                    }
                    else {
                        return std::cmp_not_equal(m_baseInt, num);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_not_equal(Base2(m_baseInt) << decDiff, num);
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less(num, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater(num, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_not_equal(m_baseInt, Base(num) << decDiff);
                    }
                }
            }
        #pragma endregion
    #pragma endregion
public:
    // >>> Constructors <<<
    #pragma region Constructors
        // Fixed point empty constructor leaves a value of 0
        constexpr FixedPoint() : m_baseInt( 0 ) {};

        // Copy constructor between FixedPoint numbers
        // If org is more precise in decimal bits, then the value gets rounded down in precision. 
        // TODO: If rounding is added target this
        template <Int Base2, uint8 DecimalBits2, bool FlowGuard2>
        constexpr FixedPoint(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& org) {
            // Makes sure negative signed FixedPoint does not get casted into an unsigned fixed point 
            ASSERT_LOG(std::is_signed_v<Base> || org.m_baseInt >= 0, "Negative fixed point casted into unsigned fixed point number");
            if constexpr (std::is_same_v<Base2,Base> && DecimalBits == DecimalBits2) {
                m_baseInt = org.m_baseInt;
            }
            else {
                constexpr int16 decDiff = DecimalBits - DecimalBits2;
                if constexpr (decDiff > 0) {
                    // Makes sure that FixedPoint number does not cast into an overflow
                    ASSERT_LOG(!FlowGuard || std::cmp_greater_equal(std::numeric_limits<Base>::max() >> decDiff, org.m_baseInt), "Fixed point number [" << org.toString().c_str() << "] too high to be casted into smaller fixed point number");
                    // No need to check for negative underflow for unsigned numbers or positive
                    if constexpr (m_isSigned) {
                        constexpr Base decDiffDivisor = (1 << decDiff);
                        ASSERT_LOG(!FlowGuard || std::cmp_less_equal(std::numeric_limits<Base>::min() / decDiffDivisor,  org.m_baseInt), "Fixed point number [" << org.toString().c_str() << "] too low to be casted into smaller fixed point number"); 
                    }

                    m_baseInt = Base(org.m_baseInt) << decDiff;
                }
                else {
                    // Makes sure that FixedPoint number does not cast into an overflow
                    constexpr int16 reverseDecDiff = -decDiff;
                    // Makes sure that FixedPoint number does not cast into an overflow
                    ASSERT_LOG(!FlowGuard || std::cmp_less_equal(org.m_baseInt >> reverseDecDiff, std::numeric_limits<Base>::max()), "Fixed point number [" << org.toString().c_str() << "] too high to be casted into smaller fixed point number");
                    if constexpr (std::is_signed_v<Base2>) {
                        constexpr Base2 reverseDecDiffDivisor = (1 << reverseDecDiff);
                        // Check for negative underflow for signed numbers
                        ASSERT_LOG(!FlowGuard || std::cmp_greater_equal(org.m_baseInt / reverseDecDiffDivisor, std::numeric_limits<Base>::min()), "Fixed point number [" << org.toString().c_str() << "] too low to be casted into smaller fixed point number"); 

                        m_baseInt = org.m_baseInt / reverseDecDiffDivisor;
                    }
                    else {
                        m_baseInt = org.m_baseInt >> reverseDecDiff;
                    }
                }
            }
        };

        template <Int Base2>
        constexpr FixedPoint(Base2 base) { integerInit(base); }

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
            // Stores and collects each digit to be processed into value
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
            // Will sacrifice some precision to prevent overflow if needed
            constexpr uint8 maxDecimalPrecision = 59;
            constexpr uint8 repDecimalBit = floorRepDecBit<maxDecimalPrecision>();
            uint64 decimalPoint = 0;
            {
                const uint64 mult = (uint64(1) << repDecimalBit);

                // Adds up remainders through long addition
                for(size_t decimalIter = decimalPointStore.size() ; decimalIter > 0 ; decimalIter--) {
                    size_t decIdx = decimalIter - 1;
                    decimalPoint /= 10;
                    decimalPoint += decimalPointStore[decIdx]  * mult;

                    // Rounding logic
                    if(decimalIter == 1 && decimalPoint % 10 >= 5) {
                        decimalPoint += 10;
                    }
                }
                
                decimalPoint /= 10;
                if constexpr (DecimalBits > repDecimalBit) {
                    if constexpr (DecimalBits == 64) {
                        // Covers possibility that decimal bit would overflow before being checked
                        ASSERT_LOG(!FlowGuard || decimalPoint != (uint64(1) << maxDecimalPrecision), str << " exceeds max size after rounding");
                    }
                    decimalPoint <<= (DecimalBits - maxDecimalPrecision);
                }

                // Checks to make sure that rounding won't result in overflow
                ASSERT_LOG(!FlowGuard || !atLimit || (decimalPoint <= m_maxDecimalPoint), str << " exceeds max size after rounding");

                // Checks to see if any value at all is recorded in case of overflow at negative
                // (Two's compliment adds a value of space to the int point if decimal point is clear)
                ASSERT_LOG(!FlowGuard || !(atLimit && negative && decimalPoint > 0), str << " exceeds max size");
            }

            // Combines Integer point and decimal point
            uint64 final;
            if constexpr (m_decimalBitsMaxed) {
                final = decimalPoint;
            }
            else {
                final = (integerPoint << DecimalBits) + decimalPoint;
            }
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

    // >>> Comparison Operators (==, >, <, ect) <<< 
    #pragma region Comparison
        #pragma Fixed Point
            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator ==(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base>) {
                        return m_baseInt == rhs.m_baseInt;
                    }
                    else {
                        return std::cmp_equal(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater_equal(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && std::cmp_less_equal(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && std::cmp_equal(Base2(m_baseInt) << decDiff, rhs.m_baseInt);
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater_equal(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) && std::cmp_less_equal(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) && std::cmp_equal(m_baseInt, Base(rhs.m_baseInt) << decDiff);
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator >(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt > rhs.m_baseInt;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_greater(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && (std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_greater(Base2(m_baseInt) << decDiff, rhs.m_baseInt));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) && (std::cmp_less(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater(m_baseInt, Base(rhs.m_baseInt) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator <(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt < rhs.m_baseInt;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_less(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && (std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_less(Base2(m_baseInt) << decDiff, rhs.m_baseInt));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) && (std::cmp_greater(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_less(m_baseInt, Base(rhs.m_baseInt) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator >=(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt >= rhs.m_baseInt;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_greater_equal(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_greater_equal(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) && (std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_greater_equal(Base2(m_baseInt) << decDiff, rhs.m_baseInt));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less_equal(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) && (std::cmp_less(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater_equal(m_baseInt, Base(rhs.m_baseInt) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator <=(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return m_baseInt <= rhs.m_baseInt;
                    }
                    else if constexpr (!std::is_same_v<Base2, Base> && DecimalBits2 == DecimalBits) {
                        return std::cmp_less_equal(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less_equal(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) && (std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_less_equal(Base2(m_baseInt) << decDiff, rhs.m_baseInt));
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_greater_equal(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) && (std::cmp_greater(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_less_equal(m_baseInt, Base(rhs.m_baseInt) << decDiff));
                    }
                }
            }

            template <typename Base2, uint8 DecimalBits2, bool FlowGuard2>
            constexpr bool operator !=(const FixedPoint<Base2, DecimalBits2, FlowGuard2>& rhs) const {
                if constexpr (DecimalBits2 == DecimalBits) {
                    if constexpr (std::is_same_v<Base2, Base>) {
                        return m_baseInt != rhs.m_baseInt;
                    }
                    else {
                        return std::cmp_not_equal(m_baseInt, rhs.m_baseInt);
                    }
                }
                else {
                    if constexpr (DecimalBits2 > DecimalBits){
                        constexpr uint8 decDiff = DecimalBits2 - DecimalBits;
                        return std::cmp_less(m_baseInt, std::numeric_limits<Base2>::min() >> decDiff) || std::cmp_greater(m_baseInt, std::numeric_limits<Base2>::max() >> decDiff) || std::cmp_not_equal(Base2(m_baseInt) << decDiff, rhs.m_baseInt);
                    }
                    else {
                        constexpr uint8 decDiff = DecimalBits - DecimalBits2;
                        return std::cmp_less(rhs.m_baseInt, std::numeric_limits<Base>::min() >> decDiff) || std::cmp_greater(rhs.m_baseInt, std::numeric_limits<Base>::max() >> decDiff) || std::cmp_not_equal(m_baseInt, Base(rhs.m_baseInt) << decDiff);
                    }
                }
            }
        #pragma endregion
        #pragma Integer
        #pragma endregion
    #pragma endregion

    // >>> Basic Arithmetic Operators (+, -, /, ect) <<<
    #pragma region Arithmetic
        FixedPoint<Base,DecimalBits,FlowGuard> operator-() const {
            static_assert(m_isSigned, "Attempted to flip sign on a unsigned fixed point number");
            // Due to two's compliment if signed int is equal to min number *-1 cannot happen without overflow
            ASSERT_LOG(!FlowGuard || m_isSigned || m_baseInt != std::numeric_limits<Base>::min(), "Fixed point number [" << toString().c_str() << "] will overflow on flipped sign");

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
        // Only supports up to 18 decimal bits of precision due to overflow issues, will round to nearest 19 decimal bit representation if over. 
        // If rounding up would cause value to overflow. The decimal will round down instead.
        friend std::ostream& operator<<(std::ostream& out, const FixedPoint<Base, DecimalBits, FlowGuard>& obj) {
            return out << obj.toString().c_str();
        }

        // Converts value of fixed point into a string.
        // Only supports up to 18 bits of decimal precision due to overflow issues, will round to nearest 19 decimal bit representation if over.
        // If rounding up would cause an overflow, the decimals will be rounded down instead.
        std::string toString() const{
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
                if constexpr (m_decimalBitsMaxed) {
                    return unsignedToString(m_baseInt, true);
                }
                else {
                    return unsignedToString(m_baseInt, ((m_baseInt & m_integerPointMask) >> DecimalBits) >= m_maxIntegerPoint);
                }
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
            ASSERT_LOG(std::is_signed_v<Base2> || m_baseInt >= 0, "Negative fixed point number ["  << toString().c_str() << "] casted as a unsigned integer");
            // Makes sure number isn't too large to be casted into specified int, flow guard does not apply since int is being casted into
            ASSERT_LOG(std::cmp_less_equal(m_baseInt >> DecimalBits, std::numeric_limits<Base2>::max()), "Fixed point number [" << toString().c_str() << "] too high to be casted into smaller fixed point number");
            if(m_isSigned) {
                ASSERT_LOG(std::cmp_greater_equal(m_baseInt / m_divisor, std::numeric_limits<Base2>::min()), "Fixed point number [" << toString().c_str() << "] too low to be casted into smaller fixed point number");
            }

            if constexpr (m_isSigned) {
                // Can't bit shift for potential negative numbers because bit shift is stupid and rounds towards negative infinity xd
                return Base2(m_baseInt / m_divisor);
            }
            else {
                return Base2(m_baseInt >> DecimalBits);
            }
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
    Base getData() const {
        return m_baseInt;
    }
};

// Defines typical configurations that fixed point numbers come in
typedef FixedPoint<int32, 8, true> fixed;
typedef FixedPoint<int64, 11, true> doubleFixed;
typedef FixedPoint<uint16, 15, false> radian;

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