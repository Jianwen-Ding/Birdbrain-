#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "Vector2i.hpp"

// Represents a vector within the 3D world space.
// It is important to use this in game logic rather than floats to enforce determinism.
struct Vector3i {
    int m_x;
    int m_y;
    int m_z;

    Vector3i() : m_x(0), m_y(0) {}

    Vector3i(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {}

    Vector3i(Vector2i& baseVec) : m_x(baseVec.m_x), m_y(baseVec.m_y), m_z(0) {}

    Vector3i(Vector2i& baseVec, int z) : m_x(baseVec.m_x), m_y(baseVec.m_y), m_z(z) {}
};

#endif