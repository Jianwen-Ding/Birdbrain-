#ifndef MATHCONSTS_HPP
#define MATHCONSTS_HPP

#include "DEBUG.hpp"

#include <cstdint>
#include <array>

// Typedefs and handles that deal with the types that similar numbers come in
#pragma region Types
template <typename T>
concept Int = std::is_integral_v<T>;

template <typename T>
concept SignedInt = std::is_integral_v<T> && std::is_signed_v<T>;

template <typename T>
concept UnsignedInt = std::is_integral_v<T> && std::is_unsigned_v<T>;

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

// Typedefs that simplifies integer name
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef unsigned int uint;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#pragma endregion

// Compiler time checks for properties of the current machine
#pragma region MachineConfirmations
// Checks whether floats conform to IEEE 754 standard
template <typename T, typename B> 
constexpr bool BitEquality(T a, B b)
{
    if constexpr (sizeof(T) != sizeof(B)) {
        return false;
    }
    else {
        return std::bit_cast<B>(a) == b;
    }
}

constexpr bool FloatConform(float givenFloat, uint32 check) {
    return BitEquality(givenFloat, check);
}

constexpr bool DoubleConform(double givenFloat, uint64 check) {
    return BitEquality(givenFloat, check);
}

// Whether floats conform with IEEE754 bit representation (DOES NOT SCAN BEHAVIOR)
constexpr bool FLOAT_IEEE754_REP = FloatConform(30, 1106247680)
                    && FloatConform(122.125, 1123303424)
                    && FloatConform(0.003742218, 997539840)
                    && FloatConform(-2.4072964, 3222933797)
                    && FloatConform(-6978.147, 3319402797)
                    && FloatConform(-37450.145, 3339864613)
                    && DoubleConform(1231223342231231, 4832783693910096636u)
                    && DoubleConform(-4392301092102903, 14064519039207523822u)
                    && DoubleConform(0.2509765625, 4598192811731320832u);
#pragma endregion
#endif