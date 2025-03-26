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

        template <SignedInt T>
        static constexpr T log2(T num) {
            using V = typename std::make_unsigned<T>::type;

            ASSERT(num >= 0);
            return log2(V(num));
        }

        template <UnsignedInt T>
        static constexpr T log2(T num) {
            ASSERT(num != 0);
            return std::bit_width(num) - 1;
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