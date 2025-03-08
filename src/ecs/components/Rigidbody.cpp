#include "Rigidbody.hpp"

void RigidBodyBehavior::start(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->givenTransform = m_sceneState->Get<TransformComponent>(givenEnt);
}

void RigidBodyBehavior::update(Rigidbody* givenComp, EntityID givenEnt){
    givenComp->givenTransform->x_pos += givenComp->v_x;
    givenComp->givenTransform->y_pos += givenComp->v_y; 
}

RigidBodyBehavior::RigidBodyBehavior(std::shared_ptr<Scene> scene) : ComponentReader<Rigidbody>(scene) {}