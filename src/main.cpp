#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "bits/stdc++.h"

#include "Tetris.h"




int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::VideoMode vm(sf::Vector2u(WINDOW_W+400, WINDOW_H));
    sf::RenderWindow window(vm, "Tetris", sf::Style::Default ^ sf::Style::Resize, settings);
    window.setFramerateLimit(60);
    bool imgui_sfml_init = ImGui::SFML::Init(window);
    sf::View view;

    Tetris t; 

    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Q && event.key.control)) {
                window.close();
            }

            if(event.type == sf::Event::Resized)
            {
                view = window.getDefaultView();
                view.setSize({event.size.width, event.size.height});
                window.setView(view);
            }

            t.processEvent(event);
            //sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hi", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Hi");
        ImGui::SetWindowSize(ImVec2(400, WINDOW_H/2));
        ImGui::SetWindowPos(ImVec2(WINDOW_W + OUTLINE, 0));
        
        ImGui::End();
        

        window.clear();
        t.render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}