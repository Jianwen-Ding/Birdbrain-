    #ifndef DETMATH_HPP
    #define DETMATH_HPP

    //#include <cstdint>
    //#include <array>

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
        static int sqrt(unsigned int num);

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
        static constexpr int pos[32] {1, 2, 29, 3, 30, 15, 25, 4,
            31, 23, 21, 16, 26, 18, 5, 9, 32, 28, 14, 24, 22, 20,
            17, 8, 27, 13, 19, 7, 12, 6, 11, 10};
    };

    #endif