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

    void step();

    //////////////////// РЕНДЕР /////////////////////
    
    void render();

private:
    //////////////////// ТЕТРИС /////////////////////

    Tetramino m_t;
    sf::Clock m_clk;
    float m_delay;
    int score;
    int lines;

    ////////////////////  ЗВУК  /////////////////////

    Audio m_audio;

    //////////////////// РЕНДЕР /////////////////////

    sf::RenderWindow &m_window;

    sf::Vector2i M_TETRIS_GRID_SIZE;

    sf::Vector2i M_SIDE_INTERFACE_SIZE;
    sf::Vector2i M_SIDE_INTERFACE_POS;
    
    sf::Vector2i M_PREVIEW_SIZE;
    sf::Vector2i M_PREVIEW_CENTER_POS;

    /////////////////// ИНТЕРФЕЙС ///////////////////

    void m_showGameInterface();

};