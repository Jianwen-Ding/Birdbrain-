#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "ComponentReader.hpp"
#include "TransformComponent.hpp"
#include "TimeManager.hpp"

struct Rigidbody {
  // Cached transform to update position of
  TransformComponent* m_givenTransform{nullptr};
  // X and Y velocities
  Vector2i m_vel {0, 0};
};

class RigidBodyBehavior : public ComponentReader<Rigidbody>{

  public:
    RigidBodyBehavior(std::shared_ptr<Scene> scene);

  private:
    // Caches transform
    void start(Rigidbody* givenComp, EntityID givenEnt) override;

    // Draws the given circle based off of transform and color
    void update(Rigidbody* givenComp, EntityID givenEnt) override;
};
#endif