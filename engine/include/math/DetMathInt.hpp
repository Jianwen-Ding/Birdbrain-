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
        // TBD: A ->

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the sin of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static constexpr int sin(int angle){
            return 0;
        }

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the cos of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static constexpr int cos(int angle){
            return 0;
        }

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the tan of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static constexpr int tan(int angle) {
            return 0;
        }
        // <-

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
        // Warning: Do not use negative numbers. This is converted to unsigned so unexpected numbers will output.
        template <SignedInt T>
        static constexpr T log(T num, uint8 base) {
            using V = typename std::make_unsigned<T>::type;

            ASSERT(num >= 0);
            return log(V(num), base);
        }

        // Finds the log of the given number and floors it to an int.
        template <UnsignedInt T>
        static constexpr T log(T num, uint8 base) {
            ASSERT(base != 0);

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
    private:
        /* For future implementation
        static constexpr void createSinLookup();

        static constexpr void createCosLookup();

        static constexpr void createTanLookup();

        static constexpr void createTanLookup();

        static constexpr std::array<int, 3600> sinTable { };

        static constexpr std::array<int, 3600> cosTable { };

        static constexpr std::array<int, 3600> tanTable { };
        */
    };
    #endif