    #ifndef DETMATH_HPP
    #define DETMATH_HPP

    #include "MathConsts.hpp"

    // Stands for deterministic math. No matter the machine/circumstance all inputs should result in the same output.
    // This class is meant to help replace any non deterministic math operations present in std.
    // Use this in game logic to allow for rollback.
    class DetMath {

    public:
        // TBD: A ->

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the sin of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static int sin(int angle);

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the cos of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static int cos(int angle);

        // TBD: CURRENTLY ONLY RETURNS 0
        // Finds the tan of a given angle.
        // Angle is degrees multiplied by 10.
        // Returns expected result multiplied by 3600.
        static int tan(int angle);
        // <-

        // Finds the sqrt of the given number and floors it to an int.
        inline static uint sqrt(uint num) {
            if( num == 0 ) {
                return 0;
            }

            // Finds bit length of number in order to set up for square root
            uint v = num;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            v = (v >> 1) + 1;
            // Uses bitwise integer square algorithm
            uint bit = 1 << (pos[((v * 0x077CB531UL) & 0xFFFFFFFF) >> 27] / 2 * 2);
            uint result = 0;
            uint origN = num;

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

        // Used to find bit positions for sqrt algorithm
        // inline static constexpr int pos[32] {1, 2, 29, 3, 30, 15, 25, 4,
        //     31, 23, 21, 16, 26, 18, 5, 9, 32, 28, 14, 24, 22, 20,
        //     17, 8, 27, 13, 19, 7, 12, 6, 11, 10};

        inline static constexpr uint8 pos[32] {0, 1, 28, 2, 29, 14, 24, 3,
            30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19,
            16, 7, 26, 12, 18, 6, 11, 5, 10, 9};
    };

    #endif