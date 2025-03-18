#ifndef SCENE_HPP
#define SCENE_HPP

#include "ECSConsts.hpp"
#include "ComponentPool.hpp"

/*
 * SCENE DEFINITION + FUNCTIONALITY
 */

// Each component has its own memory pool, to have good memory
// locality. An entity's ID is the index into its own component in the
// component pool.
struct Scene {
  struct EntityEntry {
    EntityID m_id{ };  // though redundant with index in vector, required
                       // for deleting entities,
    ComponentMask m_mask{ };
  };
  std::vector<EntityEntry> m_entities{ };
  std::vector<ComponentPool*> m_componentPools;
  std::vector<unsigned int> m_freeIndices;

  // Adds a new entity to this vector of entities, and returns its
  // ID. Can only support 2^64 entities without ID conflicts.
  EntityID NewEntity();
  
  // Removes a given entity from the scene and signals to the scene the free space that was left behind
  void DestroyEntity(EntityID id);

  // Removes a component from the entity with the given EntityID
  // if the EntityID is not already removed.
  template<typename T>
  void Remove(EntityID id){
    // ensures you're not accessing an entity that has been deleted
    if (m_entities[ECSConsts::GetEntityIndex(id)].m_id != id) 
        return;

    int componentId = ECSConsts::GetId<T>();
    // Finds location of component data within the entity component pool and 
    // resets, thus removing the component from the entity
    m_entities[ECSConsts::GetEntityIndex(id)].m_mask.reset(componentId);
  }

  // Assigns the entity associated with the given entity ID in this
  // vector of entities a new instance of the given component. Then,
  // adds it to its corresponding memory pool, and returns a pointer
  // to it.
  template<typename T>
  T* Assign(EntityID id){
    int componentId = ECSConsts::GetId<T>();

    if (m_componentPools.size() <= componentId) // Not enough component pool
    {
      m_componentPools.resize(componentId + 1, nullptr);
    }
    if (m_componentPools[componentId] == nullptr) // New component, make a new pool
    {
      m_componentPools[componentId] = new ComponentPool(sizeof(T));
    }

    // Looks up the component in the pool, and initializes it with placement new
    T* pComponent = new (m_componentPools[componentId]->get(ECSConsts::GetEntityIndex(id))) T();

    m_entities[ECSConsts::GetEntityIndex(id)].m_mask.set(componentId);
    return pComponent;
  }

  // Returns the pointer to the component instance on the entity
  // associated with the given ID in this vector of entities, with the
  // given component type. Returns nullptr if that entity doesn't have
  // the given component type.
  template<typename T>
  T* Get(EntityID id){
    int componentId = ECSConsts::GetId<T>();
    if (!m_entities[ECSConsts::GetEntityIndex(id)].m_mask.test(componentId))
        return nullptr;

    T* pComponent = static_cast<T*>(m_componentPools[componentId]->get(ECSConsts::GetEntityIndex(id)));
    return pComponent;
  }
};

#endif