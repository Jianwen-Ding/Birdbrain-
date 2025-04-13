    #ifndef DETMATHINT_HPP
    #define DETMATHINT_HPP

    #include "DEBUG.hpp"
    #include "MathConsts.hpp"

    #include <bit>

    // Stands for deterministic math. No matter the machine/circumstance all inputs should result in the same output.
    // This class is meant to help replace any non deterministic math operations present in std.
    // Use this in game logic to allow for rollback.
    class DetMathInt {

    public:
        // Finds the square root of the given number and floors it to an int.
        // Warning: Do not use negative numbers. This is converted to unsigned so unexpected numbers will output.
        template <SignedInt T>
        static constexpr T sqrt(T num) {
            using V = typename std::make_unsigned<T>::type;

            ASSERT(num >= 0);
            return sqrt(V(num));
        }

        // Finds the sqrt of the given number and floors it to an int.
        template <UnsignedInt T>
        static constexpr T sqrt(T num) {
            // Potential for rounded numbers
            // T orgNum;
            // // Stores original number for rounding
            // if constexpr (Rounded) {
            //     orgNum = num;
            // }
            // Uses bitwise integer square algorithm
            T bit = T(1) << ((std::bit_width(num) - 1) / 2 * 2);
            T result = 0;
            while (bit > 0) {
                if (num >= result + bit) {
                    num -= result + bit;
                    result = (result >> 1) + bit;
                }
                else {
                    result >>= 1;
                }
                bit >>= 2;
            }

            // // Determines rounded number
            // if constexpr (Rounded) {
            //     T remainder = orgNum - (result * result);
            //     if(remainder == 0) {
            //         return result;
            //     }
            //     return remainder < result ? result : result + 1;
            // }
            return result;
        }

        // Finds the log base 2 of the given number and floors it to an int.
        // Warning: Do not use negative numbers. This is converted to unsigned so unexpected numbers will output.
        template <SignedInt T>
        static constexpr T log2(T num) {
            using V = typename std::make_unsigned<T>::type;

            ASSERT(num >= 0);
            return log2(V(num));
        }

        // Finds the log base 2 of the given number and floors it to an int.
        template <UnsignedInt T>
        static constexpr T log2(T num) {
            ASSERT(num != 0);
            return std::bit_width(num) - 1;
        }
        
        // Finds the log of the given number and floors it to an int.
        // Unlike actual log this allows for negative numbers, and will return Log_base(-num) in that scenario
        // However like actual log, the number of 0 will be restricted
        template <Int T>
        static constexpr T log(T num, uint8 base) {
            ASSERT(num != 0);

            if(base == 1) {
                return 0;
            }

            T ans = 0;
            while (num >= base) {
                num /= base;
                ans++;
            }
            return ans;
        }
    
        // Finds the power of one number to another.
        template <SignedInt T>
        static constexpr int64 pow(T num, uint8 pow) {
            int64 result = 1;
            while(pow > 0) {
                if(pow & 1) {
                    result = (result*num);
                }
                num = (num * num);
                pow >>= 1;
            }
            return result;
        }

        // Finds the power of one number to another.
        template <UnsignedInt T>
        static constexpr uint64 pow(T num, uint8 pow) {
            uint64 result = 1;
            while(pow > 0) {
                if(pow & 1) {
                    result = (result*num);
                }
                num = (num * num);
                pow >>= 1;
            }
            return result;
        }
    };
    #endif