#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "Vector2i.hpp"

// Represents a point in a 2D space as a Component
struct TransformComponent {
    Vector2i m_pos { 0, 0};
    int m_radius{500};    
};

// Does not have any innate behaviors

#endif