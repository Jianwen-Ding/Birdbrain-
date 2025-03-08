#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "ComponentReader.hpp"
#include "TransformComponent.hpp"
#include "TimeManager.hpp"

struct Rigidbody {
  // Cached transform to update position of
  TransformComponent* givenTransform{nullptr};
  // X and Y velocities
  float v_x{0.5};
  float v_y{0.5};
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