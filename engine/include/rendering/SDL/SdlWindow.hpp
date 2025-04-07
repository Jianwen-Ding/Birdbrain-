#ifndef SDLWINDOW_HPP
#define SDLWINDOW_HPP

#include "IWindow.hpp"
#include "Color.hpp"

#include "SDL2/SDL.h"

class SdlWindow : public IWindow {
private:
    SdlWindow(std::string name, int width, int height, Color bgColor);
public:

    ~SdlWindow();

    Color getBackColor() override;

    int getWindowWidth() override;

    int getWindowHeight() override;

    bool isActive() override;

    InputCycle pollEvent() override;

    void renderScene(std::shared_ptr<Scene> givenScene) override;

    class SdlWindowBuilder : IWindowBuilder<SdlWindow> {
        public:
            SdlWindowBuilder();

            ~SdlWindowBuilder() override;

            void setBuildBackColor(Color setColor) override;

            void setBuildWindowWidth(int setWidth) override;

            void setBuildWindowHeight(int setHeight) override;

            void setBuildWindowTitle(std::string setTitle) override;

            SdlWindow build() override;

            std::string m_buildName{ };
            int m_buildWidth{ 680 };
            int m_buildHeight{ 680 };
            Color m_buildBgColor{ 255, 255, 255 };
    };

    bool m_active{ true };

    // SDL Variables
    SDL_Window *m_window{ nullptr };
    SDL_Renderer *m_renderer{ nullptr };

    // Window Config
    std::string m_name{ };
    int m_width{ 680 };
    int m_height{ 680 };
    Color m_bgColor{ 0, 0, 0};

};

#endif