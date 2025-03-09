#ifndef CIRCLERENDERER_HPP
#define CIRCLERENDERER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Color.hpp"
#include "TransformComponent.hpp"
#include "ComponentReader.hpp"

struct CircleRenderer{
    // Caches transform
    TransformComponent* m_transform {nullptr};

    // Color to render as
    Color m_renderColor{Color(0,0,0)};
};

class CircleRenderBehavior : public ComponentReader<CircleRenderer>{

public:
    CircleRenderBehavior(std::shared_ptr<Scene> scene);

private:
    // Caches transform
    void start(CircleRenderer* givenComp, EntityID givenEnt) override;

    // Draws the given circle based off of transform and color
    void update(CircleRenderer* givenComp, EntityID givenEnt) override;
    
};


#endif 