#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#define WINDOW_H 800
#define WINDOW_W 600

#include "Tetris.hpp"


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::VideoMode vm(WINDOW_W, WINDOW_H);
    sf::RenderWindow window(vm, "Tetris", sf::Style::Default ^ sf::Style::Resize, settings);
    window.setFramerateLimit(60);
    bool imgui_sfml_init = ImGui::SFML::Init(window);
    sf::View view;

    Tetris t(window); 

    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Q && event.key.control)) {
                window.close();
            }

            // if(event.type == sf::Event::Resized)
            // {
            //     view = window.getDefaultView();
            //     view.setSize({event.size.width, event.size.height});
            //     window.setView(view);
            // }

            t.processEvent(event);
            //sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowDemoWindow();
        
        t.step();

        window.clear(t.getBackgroundColor());
        t.render();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}