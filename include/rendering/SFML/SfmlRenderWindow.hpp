#ifndef SFMLRENDERWINDOW_hpp
#define SFMLRENDERWINDOW_hpp

#define TIME_AVG 1000

#include "IRenderWindow.hpp"
#include "SceneView.hpp"
#include "CircleRenderer.hpp"
#include "TimeManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// The rendering system as built from SFML.
// Currently a placeholder for the Vulkan rendering system.
class SfmlRenderWindow : public IRenderWindow {
    public:

        Color getBackColor() override;

        int getWindowWidth() override;

        int getWindowHeight() override;

        class SfmlRenderWindowBuilder : public IRenderWindow::IRenderWindowBuilder<SfmlRenderWindow> {
            public:
                // Constructs an instance of the window builder with a black background,
                // 800 wide and 600 long,
                // With a title of "SFML Game"
                SfmlRenderWindowBuilder();

                ~SfmlRenderWindowBuilder() override;

                void setBuildBackColor(Color setColor) override;

                void setBuildWindowWidth(int setWidth) override;

                void setBuildWindowHeight(int setHeight) override;

                void setBuildWindowTitle(std::string setTitle) override;

                SfmlRenderWindow build() override;

                std::string title;
                Color givenColor;
                int height;
                int width;
        };

        bool isActive() override;

        // Helper function for adding button inputs through poll event
        inline void flipButtonOn(InputCycle& givenCycle, const ButtonType type) {
            // Creates input if it doesn't already exist
            if(givenCycle.buttonInputs.find(type) == givenCycle.buttonInputs.end()) {
                givenCycle.buttonInputs.insert(std::pair<ButtonType, ButtonState>(type, ButtonState()));
            }
            else if(!givenCycle.buttonInputs[type].m_endOn) {
                givenCycle.buttonInputs[type].m_flipStates += 1;
            }
            givenCycle.buttonInputs[type].m_endOn = true;
        }

        // Helper function for adding button inputs through poll event
        inline void flipButtonOff(InputCycle& givenCycle, const ButtonType type) {
            // Creates input if it doesn't already exist
            if(givenCycle.buttonInputs.find(type) == givenCycle.buttonInputs.end()) {
                givenCycle.buttonInputs.insert(std::pair<ButtonType, ButtonState>(type, ButtonState()));
            }
            else if(givenCycle.buttonInputs[type].m_endOn) {
                givenCycle.buttonInputs[type].m_flipStates += 1;
            }
            givenCycle.buttonInputs[type].m_endOn = false;
        }


        InputCycle pollEvent() override;

        void renderScene(std::shared_ptr<Scene> givenScene) override;

        ~SfmlRenderWindow() override;

    protected:
        // Constructs a window that uses SFML.
        SfmlRenderWindow(Color setBackground, int setHeight, int setWidth, std::string setTitle);

        sf::RenderWindow mainWindow;
        sf::Color givenBackground;
        int height;
        int width;
        sf::Font profilerFont;
        std::vector<int> frameTimes;
};

#endif