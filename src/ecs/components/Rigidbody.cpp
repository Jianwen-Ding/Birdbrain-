#include "Rigidbody.hpp"

void RigidBodyBehavior::start(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void RigidBodyBehavior::update(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->m_givenTransform->m_pos.m_x += givenComp->m_vel.m_x;
    givenComp->m_givenTransform->m_pos.m_y += givenComp->m_vel.m_y; 
}

RigidBodyBehavior::RigidBodyBehavior(std::shared_ptr<Scene> scene) : ComponentReader<Rigidbody>(scene) {}