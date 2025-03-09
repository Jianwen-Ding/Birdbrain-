#include "CircleRenderer.hpp"

void CircleRenderBehavior::start(CircleRenderer* givenComp, EntityID givenEnt){
    givenComp->m_transform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void CircleRenderBehavior::update(CircleRenderer* givenComp, EntityID givenEnt){
    // Render to window
    sf::CircleShape shape(givenComp->m_transform->m_radius);
    shape.setFillColor(sf::Color(100, 250, 50));

    shape.setPosition(givenComp->m_transform->m_pos.m_x, givenComp->m_transform->m_pos.m_y);
    
    // givenComp->window->draw(shape);
}

// Passes itself into ECSManager
CircleRenderBehavior::CircleRenderBehavior(std::shared_ptr<Scene> scene) : ComponentReader<CircleRenderer>(scene) {}

// CircleRenderBehavior CircleRenderBehavior::base = CircleRenderBehavior();
