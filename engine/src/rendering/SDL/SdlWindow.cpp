#include "SdlWindow.hpp"

#include "SceneView.hpp"
#include "TransformComponent.hpp"
#include "CircleRenderer.hpp"

SdlWindow::SdlWindow(std::string name, int width, int height, Color m_bgColor) { 
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Hitting this means failure to initialize SDL
        ASSERT(false);
    }

    m_window = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        0);

    if(!m_window) {
        // Hitting this means failure to make window
        ASSERT(false);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    if(!m_renderer) {
        // Hitting this means failure to make renderer
        ASSERT(false);
    }

    SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, 255);
    SDL_RenderClear(m_renderer);
}

SdlWindow::~SdlWindow() {

}

Color SdlWindow::getBackColor() {
    return m_bgColor;
}

int SdlWindow::getWindowWidth() {
    return m_width;
}

int SdlWindow::getWindowHeight() {
    return m_height;
}

bool SdlWindow::isActive() {
    return m_active;
}

InputCycle SdlWindow::pollEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            m_active = false;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
        }
    }
    return InputCycle();
}

void SdlWindow::renderScene(std::shared_ptr<Scene> givenScene) {
    // Finds the position of each circle and represents them as a square (Switching from SFML problems lmao)
    SceneView<CircleRenderer, TransformComponent> sceneViewer = SceneView<CircleRenderer, TransformComponent>(givenScene);
    for (EntityID ent : sceneViewer) {
        CircleRenderer* circleRep = givenScene->Get<CircleRenderer>(ent);
        SDL_SetRenderDrawColor(m_renderer, circleRep->m_renderColor.r, circleRep->m_renderColor.g, circleRep->m_renderColor.b, 255);

        TransformComponent* transformRep = circleRep->m_transform;
        constexpr float divisor = 32.0f;
        SDL_Rect rect = {(int)(transformRep->m_pos.m_x / divisor), (int)(transformRep->m_pos.m_y / divisor), (int)(transformRep->m_radius / divisor), (int)(transformRep->m_radius / divisor)};
        SDL_RenderFillRect(m_renderer, &rect);
    }
    // Sets background color
    SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, 255);

    // Actually presents
    SDL_RenderPresent(m_renderer);
    SDL_RenderClear(m_renderer);
}

void SdlWindow::SdlWindowBuilder::setBuildBackColor(Color setColor) {
    m_buildBgColor = setColor;
}

void SdlWindow::SdlWindowBuilder::setBuildWindowWidth(int setWidth) {
    m_buildWidth = setWidth;
}

void SdlWindow::SdlWindowBuilder::setBuildWindowHeight(int setHeight) {
    m_buildHeight = setHeight;
}

void SdlWindow::SdlWindowBuilder::setBuildWindowTitle(std::string setTitle) {
    m_buildName = setTitle;
}

SdlWindow SdlWindow::SdlWindowBuilder::build() {
    return SdlWindow(m_buildName, m_buildWidth, m_buildHeight, m_buildBgColor);
}

SdlWindow::SdlWindowBuilder::SdlWindowBuilder() { }
SdlWindow::SdlWindowBuilder::~SdlWindowBuilder() { }