#ifndef COMPONENTREADER_HPP
#define COMPONENTREADER_HPP

#include "SceneView.hpp"
#include "ComponentReaderBase.hpp"
#include "SceneManager.hpp"
// This allows for an easier implementation of behaviors into the ECS as behaviors are
// plugged in just by defining the correct types
// The ComponentType is the component it processes
// The BaseType is the class that inherits this, requires a empty constructor
template<typename ComponentType>
class ComponentReader : public ComponentReaderBase<> {
    public:
        // Uses the implemented behavior on every component
        void fullStart() override {
            for (EntityID ent : m_givenSceneView) {
                start(m_givenSceneView.m_pScene->template Get<ComponentType>(ent), ent);
            }
        }

        // Uses the implemented behavior on every component
        void fullUpdate() override {
            for (EntityID ent : m_givenSceneView) {
                update(m_givenSceneView.m_pScene->template Get<ComponentType>(ent), ent);
            }
        }

        ComponentReader(std::shared_ptr<Scene> sceneState) : ComponentReaderBase(sceneState), m_givenSceneView(sceneState) {}

        virtual ~ComponentReader() = default;
        
    protected:
        // The behavior that a component initiates when the ECSManager actually starts the game
        virtual void start(ComponentType* givenComp, EntityID givenEnt) = 0;
        // The behavior that a component initiates every frame
        virtual void update(ComponentType* givenComp, EntityID givenEnt) = 0;

        // The Scene to gleam relevant components to update from
        SceneView<ComponentType> m_givenSceneView;
};
#endif