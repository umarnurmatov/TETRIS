#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tetramino.h"

class Tetris
{
    Tetramino t;
    sf::Clock clk;
    float delay;

public:
    Tetris()
        : delay{DELAY}
    {
        t.genTetramino();
        t.update();
        clk.restart();
    }

    void processEvent(sf::Event& event);

    void render(sf::RenderWindow &window);
};