#ifndef COMPONENTPOOL_HPP
#define COMPONENTPOOL_HPP

#include "ECSConsts.hpp"

// Responsible for allocating contiguous memory for the components
// such that `MAX_ENTITIES` can be stored, and components be accessed
// via index.
// NOTE: The memory pool is an array of bytes, as the size of one
// component isn't known at compile time.
struct ComponentPool
{
  ComponentPool(size_t elementsize);

  ~ComponentPool();

  // Gets the component in this pData at the given index.
  inline void* get(size_t index)
  {
    // looking up the component at the desired index
    return m_pData + index * m_elementSize;
  }

  uint8* m_pData{ nullptr };
  size_t m_elementSize{ 0 };
};

#endif