#ifndef COMPONENTREADERBASE_HPP
#define COMPONENTREADERBASE_HPP

#include "Scene.hpp"
#include "SceneView.hpp"

// This acts as an interface for the behavior a Component
// This allows for this class to be passed into the ECSManager regardless of type of component being searched for
// and prevents circular dependency
template<typename... Args>
class ComponentReaderBase {
    public:
        // Uses the implemented behavior on every component
        virtual void fullStart() = 0;

        // Uses the implemented behavior on every component
        virtual void fullUpdate(Args... args) = 0;

        ComponentReaderBase(std::shared_ptr<Scene> setScene) {
            m_sceneState = setScene;
        }

        virtual ~ComponentReaderBase() = default;

    protected:
        std::shared_ptr<Scene> m_sceneState;
};

#endif