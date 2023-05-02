#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tetramino.hpp"
#include "Audio.hpp"


class Tetris
{
    Tetramino t;
    sf::Clock clk;
    float delay;

    Audio audio;
public:
    Tetris()
        : delay{DELAY}
    {
        t.genTetramino();
        t.update();
        clk.restart();
        audio.playMain();
    }

    void processEvent(sf::Event& event);

    void render(sf::RenderWindow &window);
};