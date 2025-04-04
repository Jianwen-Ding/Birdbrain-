#ifndef COLOR_HPP
#define COLOR_HPP

#include "MathConsts.hpp"

// Represents a color to be rendered
// Always a solid color
struct Color {
    // Red value of the color
    uint8 r{ 0 };
    // Green value of the color
    uint8 g{ 0 };
    // Blue value of the color
    uint8 b{ 0 };

    // Constructs a color with the given RGB values
    Color(uint8 r, uint8 g, uint8 b) : r(r), g(g), b(b) {};

    // Constructs a pure black color
    Color() {};
};

#endif 