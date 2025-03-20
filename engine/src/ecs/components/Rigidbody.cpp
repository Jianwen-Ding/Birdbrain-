#include "Rigidbody.hpp"

#include <iostream>

void RigidBodyBehavior::start(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void RigidBodyBehavior::update(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform->m_pos = givenComp->m_givenTransform->m_pos + givenComp->m_vel;
    std::cout << "X: " << givenComp->m_givenTransform->m_pos.m_x  << " Y: " << givenComp->m_givenTransform->m_pos.m_y << std::endl;
    std::cout << "X vel: " << givenComp->m_givenTransform->m_pos.m_x  << " Y vel: " << givenComp->m_givenTransform->m_pos.m_y << std::endl;
}

RigidBodyBehavior::RigidBodyBehavior(std::shared_ptr<Scene> scene) : ComponentReader<Rigidbody>(scene) {}