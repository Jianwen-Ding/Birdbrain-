#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <vector>
#include <bitset>
#include <queue>

// for the ball game we make with the ECS
#define BALL_RADIUS 300
#define NUM_BALLS 10

// Loads ECS System
#include "ComponentPool.hpp"
#include "ECSConsts.hpp"
#include "Scene.hpp"
#include "SceneView.hpp"

// Loads Components
#include "CircleCollider.hpp"
#include "CircleRenderer.hpp"
#include "Rigidbody.hpp"
#include "TransformComponent.hpp"

// Loads Renderer
#include "SfmlRenderWindow.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Generates a random float in the inclusive range of the two given
// floats.
float RandInBetween(float LO, float HI)
{
  // From https://stackoverflow.com/questions/686353/random-float-number-generation
  return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

int main() {
  SfmlRenderWindow::SfmlRenderWindowBuilder builder;
  builder.setBuildWindowWidth(WINDOW_WIDTH);
  builder.setBuildWindowHeight(WINDOW_HEIGHT);
  SfmlRenderWindow window = builder.build();
  srand (static_cast <unsigned> (time(0)));
  
  std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
  // Instantiate all the balls.
  for (uint32 i = 0; i < NUM_BALLS; i++)
  {
    EntityID ball = scene->NewEntity();
    CircleRenderer* pBallRender = scene->Assign<CircleRenderer>(ball);
    TransformComponent* pBallTransform = scene->Assign<TransformComponent>(ball);
    Rigidbody* pBallRb = scene->Assign<Rigidbody>(ball);
    CircleCollider* pBallCC = scene->Assign<CircleCollider>(ball);
    float radius = BALL_RADIUS;
    
    pBallRender->m_renderColor = Color(255,255,255);
    pBallRender->m_transform = pBallTransform;
    pBallTransform->m_pos.m_x = RandInBetween(radius * 2, (WINDOW_WIDTH * 32) - radius * 2);
    pBallTransform->m_pos.m_y = RandInBetween(radius * 2, (WINDOW_HEIGHT * 32) - radius * 2);
    pBallRb->m_vel.m_x = RandInBetween(10, 13);
    pBallRb->m_vel.m_y = RandInBetween(10, 13);
    pBallTransform->m_radius = radius;

  }
  SceneManager sceneManager = SceneManager(scene);
  sceneManager.insertReader<CircleColliderBehavior>();
  sceneManager.insertReader<RigidBodyBehavior>();
  TimeManager::init();

  sceneManager.start();
  
  // run the program as long as the window is open
  while (window.isActive())
  {
    window.pollEvent();
    sceneManager.update();
    TimeManager::update();
    window.renderScene(scene);
  }
}

