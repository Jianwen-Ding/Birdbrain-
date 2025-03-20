#include "SfmlRenderWindow.hpp"
#include <iostream>

// Main functions
Color SfmlRenderWindow::getBackColor(){
    return Color(givenBackground.r, givenBackground.g, givenBackground.b);
}

int SfmlRenderWindow::getWindowWidth(){
    return width;
}

int SfmlRenderWindow::getWindowHeight(){
    return height;
}

bool SfmlRenderWindow::isActive(){
    return mainWindow.isOpen();
}

// CURRENTLY RETURNS NOTHING
InputCycle SfmlRenderWindow::pollEvent(){
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;

    InputCycle retCycle;

    while (mainWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed) {
            mainWindow.close();
        }
    }
    return InputCycle();
}


void SfmlRenderWindow::renderScene(std::shared_ptr<Scene> givenScene){
    mainWindow.clear(givenBackground);
    
    SceneView<CircleRenderer> scene = SceneView<CircleRenderer>(givenScene);
    for (EntityID ent : scene)
    {
        CircleRenderer* givenRenderer = givenScene->Get<CircleRenderer>(ent);
        
        sf::CircleShape shape(givenRenderer->m_transform->m_radius / 32.0f);

        shape.setFillColor(sf::Color(givenRenderer->m_renderColor.r, givenRenderer->m_renderColor.g, givenRenderer->m_renderColor.b));

        shape.setPosition(givenRenderer->m_transform->m_pos.m_x / 32.0f, givenRenderer->m_transform->m_pos.m_y / 32.0f);
        
        mainWindow.draw(shape);
    }

    // Profiler logic
    int framesPerSec = (int)(1 / TimeManager::getDelta());
    frameTimes.push_back(framesPerSec);
    if(frameTimes.size() > TIME_AVG){
        frameTimes.erase(frameTimes.begin());
    }
    int sum = 0;
    for (unsigned int i = 0; i < frameTimes.size(); i++){
      sum += frameTimes[i];
    }
    int avgFrames = sum / frameTimes.size();
    std::string perAvgFrame = "Frames per second Averaged Across " + std::to_string(TIME_AVG) + " Frames: " + std::to_string(avgFrames);
    std::string perFrame = "Frames per second: " + std::to_string(framesPerSec);

    // Renders profiler
    sf::Text profiler = sf::Text();
    sf::Text avgProfiler = sf::Text();
    profiler.setString(perFrame);
    profiler.setFillColor(sf::Color::Green);
    avgProfiler.setString(perAvgFrame);
    avgProfiler.setFillColor(sf::Color::Green);
    profiler.setFont(profilerFont);
    avgProfiler.setFont(profilerFont);
    profiler.setCharacterSize(20);
    avgProfiler.setCharacterSize(20);
    profiler.setPosition(sf::Vector2f(50,100));
    avgProfiler.setPosition(sf::Vector2f(50,50));

    mainWindow.draw(profiler);
    mainWindow.draw(avgProfiler);

    // end the current frame
    mainWindow.display();
}


SfmlRenderWindow::SfmlRenderWindow(Color setBackground, int setHeight, int setWidth, std::string setString) : 
    givenBackground(setBackground.r, setBackground.g, setBackground.b), 
    height(setHeight), 
    width(setWidth) ,
    mainWindow(sf::VideoMode(setWidth, setHeight), setString){
    profilerFont.loadFromFile("../../resources/cour.ttf");
}

SfmlRenderWindow::~SfmlRenderWindow(){
}


// Builder functions
void SfmlRenderWindow::SfmlRenderWindowBuilder::setBuildBackColor(Color setColor){
    givenColor = setColor;
}

void SfmlRenderWindow::SfmlRenderWindowBuilder::setBuildWindowWidth(int setWidth){
    width = setWidth;
}

void SfmlRenderWindow::SfmlRenderWindowBuilder::setBuildWindowHeight(int setHeight){
    height = setHeight;
}

void SfmlRenderWindow::SfmlRenderWindowBuilder::setBuildWindowTitle(std::string setTitle){
    title = std::string(setTitle);
}

SfmlRenderWindow SfmlRenderWindow::SfmlRenderWindowBuilder::build(){
    return SfmlRenderWindow(givenColor, height, width, title);
}

SfmlRenderWindow::SfmlRenderWindowBuilder::SfmlRenderWindowBuilder() :
    givenColor(0,0,0),
    width (800),
    height (600),
    title ("SFML Game"){
}

SfmlRenderWindow::SfmlRenderWindowBuilder::~SfmlRenderWindowBuilder(){
}