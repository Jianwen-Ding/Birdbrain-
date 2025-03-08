#ifndef SCENEVIEW_HPP
#define SCENEVIEW_HPP

#include "ECSConsts.hpp"
#include "Scene.hpp"

// Helps with iterating through a given scene
template<typename... ComponentTypes>
struct SceneView
{
  SceneView(std::shared_ptr<Scene> scene)  : pScene(scene) 
  {
    if (sizeof...(ComponentTypes) == 0)
    {
      all = true;
    }
    else
    {
      // Unpack the template parameters into an initializer list
      int componentIds[] = { 0, ECSConsts::GetId<ComponentTypes>() ... };
      for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
        componentMask.set(componentIds[i]);
    }
  }

  struct Iterator
  {
    Iterator(std::shared_ptr<Scene> pScene, unsigned int index, ComponentMask mask, bool all) 
    : pScene(pScene), index(index), mask(mask), all(all) {}

    // give back the entityID we're currently at
    EntityID operator*() const{
      return pScene->m_entities[index].m_id; 
    }

    // Compare two iterators
    bool operator==(const Iterator& other) const{
      return index == other.index || index == pScene->m_entities.size();
    }
    bool operator!=(const Iterator& other) const{
      return index != other.index && index != pScene->m_entities.size();
    }

    bool ValidIndex(){
      return
      // It's a valid entity ID
      ECSConsts::IsEntityValid(pScene->m_entities[index].m_id) &&
      // It has the correct component mask
      (all || mask == (mask & pScene->m_entities[index].m_mask));
    }

    // Move the iterator forward
    Iterator& operator++(){
      do
      {
        index++;
      } while (index < pScene->m_entities.size() && !ValidIndex());
      return *this;
    }
    unsigned int index;
    std::shared_ptr<Scene> pScene;
    ComponentMask mask;
    bool all{ false };
  };

  // Give an iterator to the beginning of this view
  const Iterator begin() const{
    int firstIndex = 0;
    while (firstIndex < pScene->m_entities.size() &&
      (componentMask != (componentMask & pScene->m_entities[firstIndex].m_mask) 
        || !ECSConsts::IsEntityValid(pScene->m_entities[firstIndex].m_id))) 
    {
      firstIndex++;
    }
    return Iterator(pScene, firstIndex, componentMask, all);
  }

  // Give an iterator to the end of this view 
  const Iterator end() const{
    return Iterator(pScene, (unsigned int)(pScene->m_entities.size()), componentMask, all);
  }

  std::shared_ptr<Scene> pScene{ nullptr };
  ComponentMask componentMask;
  bool all{ false };
};

#endif