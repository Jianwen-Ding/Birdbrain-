#include "CircleCollider.hpp"

#include "DetMathInt.hpp"

void CircleColliderBehavior::start(CircleCollider* givenComp, EntityID givenEnt) {
    givenComp->m_transform = m_givenSceneView.m_pScene->Get<TransformComponent>(givenEnt);
    givenComp->m_rigidbody = m_givenSceneView.m_pScene->Get<Rigidbody>(givenEnt);
}

void CircleColliderBehavior::update(CircleCollider* givenComp, EntityID givenEnt) {
  for (EntityID ent : m_givenSceneView)
  {
    CircleCollider* cc = m_sceneState->Get<CircleCollider>(ent);
    if(ent != givenEnt){
      TransformComponent* otherTransform = cc->m_transform;
      TransformComponent* transform = givenComp->m_transform;
      Rigidbody* otherRigidbody = cc->m_rigidbody;
      Rigidbody* rigidbody = givenComp->m_rigidbody;
      Vector2i diff = otherTransform ->m_pos - transform->m_pos;
      int distance = diff.mag();
      if(distance < otherTransform->m_radius + transform->m_radius){
        Vector2i norm = diff * 512 / distance;
        int thisSpeedMag = -rigidbody->m_vel.mag();
        rigidbody->m_vel= (norm * thisSpeedMag) / 512;
        int speedMag = otherRigidbody->m_vel.mag();
        otherRigidbody->m_vel = (norm * speedMag) / 512;
      }
    }
  }

  // Collision check x-axis
  if ((givenComp->m_transform->m_pos.m_x - givenComp->m_transform->m_radius) < 0) {
    givenComp->m_transform->m_pos.m_x = givenComp->m_transform->m_radius;
    givenComp->m_rigidbody->m_vel.m_x *= -1;
  }
  else if((givenComp->m_transform->m_pos.m_x + givenComp->m_transform->m_radius) > 800 * 32) {
    givenComp->m_transform->m_pos.m_x = 800 * 32 - givenComp->m_transform->m_radius;
    givenComp->m_rigidbody->m_vel.m_x *= -1;
  }

  // Collision check y-axis
  if ((givenComp->m_transform->m_pos.m_y - givenComp->m_transform->m_radius) < 0) {
    givenComp->m_transform->m_pos.m_y = givenComp->m_transform->m_radius;
    givenComp->m_rigidbody->m_vel.m_y *= -1;
  }
  else if((givenComp->m_transform->m_pos.m_y + givenComp->m_transform->m_radius) > 600 * 32) {
    givenComp->m_transform->m_pos.m_y = 600 * 32 - givenComp->m_transform->m_radius;
    givenComp->m_rigidbody->m_vel.m_y *= -1;
  }

}

CircleColliderBehavior::CircleColliderBehavior(std::shared_ptr<Scene> scene) : ComponentReader<CircleCollider>(scene){
}
