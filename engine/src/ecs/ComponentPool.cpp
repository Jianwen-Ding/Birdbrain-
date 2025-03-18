#include "ComponentPool.hpp"

ComponentPool::ComponentPool(size_t elementsize)
{
    // We'll allocate enough memory to hold MAX_ENTITIES, each with element size
    m_elementSize = elementsize;
    m_pData = new uint8[m_elementSize * MAX_ENTITIES];
}

ComponentPool::~ComponentPool()
{
    delete[] m_pData;
}