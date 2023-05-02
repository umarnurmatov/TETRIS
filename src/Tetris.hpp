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
public:
    Tetris();

    //////////////////// ИВЕНТЫ /////////////////////

    void processEvent(sf::Event& event);

    //////////////////// ТЕТРИС /////////////////////

    /// @brief <по пробелу> тетрамино сразу падает вниз
    void immediateFall();
    void step();

    //////////////////// РЕНДЕР /////////////////////
    void render(sf::RenderWindow &window);

private:
    //////////////////// ТЕТРИС /////////////////////

    Tetramino t;
    sf::Clock clk;
    float delay;

    ////////////////////  ЗВУК  /////////////////////

    Audio audio;
};