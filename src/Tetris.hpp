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
    Tetris(sf::RenderWindow& window);

    //////////////////// ИВЕНТЫ /////////////////////

    void processEvent(sf::Event& event);

    //////////////////// ТЕТРИС /////////////////////

    /// @brief <по пробелу> тетрамино сразу падает вниз
    void immediateFall();
    void step();

    //////////////////// РЕНДЕР /////////////////////
    void render();

private:
    //////////////////// ТЕТРИС /////////////////////

    Tetramino m_t;
    sf::Clock m_clk;
    float m_delay;

    ////////////////////  ЗВУК  /////////////////////

    Audio m_audio;

    //////////////////// РЕНДЕР /////////////////////

    sf::RenderWindow &m_window;
    void m_renderPreview();
};