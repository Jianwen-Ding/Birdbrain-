#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "DetMath.hpp"

// Represents a vector within the 2D world space.
struct Vector2i {
    int m_x;
    int m_y;

    Vector2i() : m_x(0), m_y(0) {}

    Vector2i(int x, int y) : m_x(x), m_y(y) {}

    // Gets the magnitude of the current vector
    inline int mag() {
        return DetMath::sqrt(this->m_x * this->m_x + this->m_y * this->m_y);
    }

    Vector2i operator-(Vector2i v2) {
        return Vector2i(this->m_x - v2.m_x, this->m_y - v2.m_y);;
    }

    Vector2i operator+(Vector2i v2) {
        return Vector2i(v2.m_x + this->m_x, v2.m_y + this->m_y);;
    }

    Vector2i operator*(Vector2i v2) {
        return Vector2i(v2.m_x * this->m_x, v2.m_y * this->m_y);;
    }

    Vector2i operator/(Vector2i v2) {
        return Vector2i(this->m_x / v2.m_x, this->m_y / v2.m_y);;
    }

    Vector2i operator+(int i2) {
        return Vector2i(i2+ this->m_x, i2 + this->m_y);;
    }

    Vector2i operator*(int i2) {
        return Vector2i(i2 * this->m_x, i2 * this->m_y);;
    }

    Vector2i operator/(int v2) {
        return Vector2i(this->m_x / v2, this->m_y / v2);;
    }
};

#endif