#ifndef CIRCLECOLLIDER_HPP
#define CIRCLECOLLIDER_HPP

#include "ComponentReader.hpp"
#include "TransformComponent.hpp"
#include "Rigidbody.hpp"


struct CircleCollider{
    // Caches the transform of the collider to find location of colliders
    // Also used to determine radius of collider
    TransformComponent* transform{nullptr};
    // Caches the rigidbody of the collider to update velocity on collisions
    Rigidbody* rigidbody;
};

// Scans for collisions and properly accordingly updates the cached rigidbodies
class CircleColliderBehavior : public ComponentReader<CircleCollider>
{

public:
    // Passes itself into ECSManager
    CircleColliderBehavior(std::shared_ptr<Scene> scene);

private:
    // Caches transform and rigidbody
    void start(CircleCollider* givenComp, EntityID givenEnt) override;

    // Checks for collisions, updating rigidbody on collisions
    void update(CircleCollider* givenComp, EntityID givenEnt) override;
};

#endif
