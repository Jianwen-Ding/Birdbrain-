#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "DetMath.hpp"

// Represents a vector within the 2D world space.
struct Vector2i {
    int m_x{ 0 };
    int m_y{ 0 };

    Vector2i() : m_x(0), m_y(0) {}

    Vector2i(int x, int y) : m_x(x), m_y(y) {}

    // Gets the magnitude of the current vector
    inline uint mag() const {
        return DetMath::sqrt(this->m_x * this->m_x + this->m_y * this->m_y);
    }

    Vector2i operator-(Vector2i v2) const {
        return Vector2i(this->m_x - v2.m_x, this->m_y - v2.m_y);;
    }

    Vector2i operator+(Vector2i v2) const {
        return Vector2i(v2.m_x + this->m_x, v2.m_y + this->m_y);;
    }

    Vector2i operator*(Vector2i v2) const {
        return Vector2i(v2.m_x * this->m_x, v2.m_y * this->m_y);;
    }

    Vector2i operator/(Vector2i v2) const {
        return Vector2i(this->m_x / v2.m_x, this->m_y / v2.m_y);;
    }

    Vector2i operator+(int i2) const {
        return Vector2i(i2+ this->m_x, i2 + this->m_y);;
    }

    // TODO: Make operation the inverse way possible
    Vector2i operator*(int i2) const {
        return Vector2i(i2 * this->m_x, i2 * this->m_y);;
    }

    Vector2i operator/(int v2) const {
        return Vector2i(this->m_x / v2, this->m_y / v2);;
    }

    bool operator==(const Vector2i v2) const {
        return this->m_x == v2.m_x
            && this->m_y == v2.m_y;
    }

    bool operator!=(const Vector2i v2) const {
        return this->m_x != v2.m_x
            || this->m_y != v2.m_y;
    }
};

#endif