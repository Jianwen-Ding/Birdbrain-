#include "CircleRenderer.hpp"

void CircleRenderBehavior::start(CircleRenderer* givenComp, EntityID givenEnt){
    givenComp->m_transform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void CircleRenderBehavior::update(CircleRenderer* givenComp, EntityID givenEnt){
}

// Passes itself into ECSManager
CircleRenderBehavior::CircleRenderBehavior(std::shared_ptr<Scene> scene) : ComponentReader<CircleRenderer>(scene) {}

// CircleRenderBehavior CircleRenderBehavior::base = CircleRenderBehavior();
