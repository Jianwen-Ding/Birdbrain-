#include "Scene.hpp"

EntityID Scene::NewEntity() {
    // std::vector::size runs in constant time.
    if (!m_freeIndices.empty()){
        unsigned int newIndex = m_freeIndices.back();
        m_freeIndices.pop_back();
        // Takes in index and incremented EntityVersion at that index
        EntityID newID = ECSConsts::CreateEntityId(newIndex, ECSConsts::GetEntityVersion(m_entities[newIndex].m_id));
        m_entities[newIndex].m_id = newID;
        return m_entities[newIndex].m_id;
    }
    m_entities.push_back({ ECSConsts::CreateEntityId((unsigned int)(m_entities.size()), 0), ComponentMask() });
    return m_entities.back().m_id;
}

// Removes a given entity from the scene and signals to the scene the free space that was left behind
void Scene::DestroyEntity(EntityID id) {
    // Increments EntityVersion at the deleted index
    EntityID newID = ECSConsts::CreateEntityId((unsigned int)(-1), ECSConsts::GetEntityVersion(id) + 1);
    m_entities[ECSConsts::GetEntityIndex(id)].m_id = newID;
    m_entities[ECSConsts::GetEntityIndex(id)].m_mask.reset(); 
    m_freeIndices.push_back(ECSConsts::GetEntityIndex(id));
}