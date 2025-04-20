#ifndef SCENEVIEW_HPP
#define SCENEVIEW_HPP

#include "ECSConsts.hpp"
#include "Scene.hpp"

// Helps with iterating through a given scene
template<typename... ComponentTypes>
struct SceneView
{
  SceneView(std::shared_ptr<Scene> scene)  : m_pScene(scene) 
  {
    if (sizeof...(ComponentTypes) == 0)
    {
      m_all = true;
    }
    else
    {
      // Unpack the template parameters into an initializer list
      int componentIds[] = { 0, ECSConsts::GetId<ComponentTypes>() ... };
      for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
        m_componentMask.set(componentIds[i]);
    }
  }

  struct Iterator
  {
    Iterator(std::shared_ptr<Scene> pScene, unsigned int index, ComponentMask mask, bool all) 
    : m_index(index), m_pScene(pScene), m_mask(mask), m_all(all) {}

    // give back the entityID we're currently at
    EntityID operator*() const{
      return m_pScene->m_entities[m_index].m_id; 
    }

    // Compare two iterators
    bool operator==(const Iterator& other) const{
      return m_index == other.m_index || m_index == m_pScene->m_entities.size();
    }
    bool operator!=(const Iterator& other) const{
      return m_index != other.m_index && m_index != m_pScene->m_entities.size();
    }

    bool ValidIndex(){
      return
      // It's a valid entity ID
      ECSConsts::IsEntityValid(m_pScene->m_entities[m_index].m_id) &&
      // It has the correct component mask
      (m_all || m_mask == (m_mask & m_pScene->m_entities[m_index].m_mask));
    }

    // Move the iterator forward
    Iterator& operator++(){
      do
      {
        m_index++;
      } while (m_index < m_pScene->m_entities.size() && !ValidIndex());
      return *this;
    }
    unsigned int m_index;
    std::shared_ptr<Scene> m_pScene;
    ComponentMask m_mask;
    bool m_all{ false };
  };

  // Give an iterator to the beginning of this view
  const Iterator begin() const{
    int firstIndex = 0;
    while (firstIndex < m_pScene->m_entities.size() &&
      (m_componentMask != (m_componentMask & m_pScene->m_entities[firstIndex].m_mask) 
        || !ECSConsts::IsEntityValid(m_pScene->m_entities[firstIndex].m_id))) 
    {
      firstIndex++;
    }
    return Iterator(m_pScene, firstIndex, m_componentMask, m_all);
  }

  // Give an iterator to the end of this view 
  const Iterator end() const{
    return Iterator(m_pScene, (unsigned int)(m_pScene->m_entities.size()), m_componentMask, m_all);
  }

  std::shared_ptr<Scene> m_pScene{ nullptr };
  ComponentMask m_componentMask;
  bool m_all{ false };
};

#endif