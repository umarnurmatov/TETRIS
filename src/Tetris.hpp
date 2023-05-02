#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tetramino.hpp"
#include "Audio.hpp"

// в секундах
#define DELAY 0.5f
#define DELAY_FAST 0.1f

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

    /// @brief <по пробелу> тетрамино сразу падает вниз
    void immediateFall();
    void step();

    void render(sf::RenderWindow &window);
};