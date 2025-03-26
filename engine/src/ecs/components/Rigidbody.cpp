#include "Rigidbody.hpp"

#include <iostream>

void RigidBodyBehavior::start(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void RigidBodyBehavior::update(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform->m_pos = givenComp->m_givenTransform->m_pos + givenComp->m_vel;
}

RigidBodyBehavior::RigidBodyBehavior(std::shared_ptr<Scene> scene) : ComponentReader<Rigidbody>(scene) {}