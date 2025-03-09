#include "DetMath.hpp"

// TBD: CURRENTLY ONLY RETURNS 0
int DetMath::sin(int angle) {
    return 0;
}

// TBD: CURRENTLY ONLY RETURNS 0
int DetMath::cos(int angle) {
    return 0;
}

// TBD: CURRENTLY ONLY RETURNS 0
int DetMath::tan(int angle) {
    return 0;
}

// Uses bitwise integer square algorithm
int DetMath::sqrt(unsigned int num) {
    if(num == 0) {
        return 0;
    }

    // Preps for the finding of bitlength (the amount of bits required to represent the number)
    unsigned int v = num;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v = (v >> 1) + 1;

    // Uses bitlength to find floored sqrt with
    unsigned int bit = 1 << (pos[((v * 0x077CB531UL) & 0xFFFFFFFF) >> 27] / 2 * 2);
    unsigned int result = 0;
    unsigned int origN = num;
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