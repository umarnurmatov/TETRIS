#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tetramino.hpp"
#include "Audio.hpp"

// в мс
#define DELAY_INIT 500.f
#define DELAY_FAST_INIT 100.f
#define DELAY_DELTA 50.f // вычитается из delay с каждым новым уровнем

#define NEW_LEVEL_SCORE 1000

#define LINE_1_SCORE 100
#define LINE_2_SCORE 300
#define LINE_3_SCORE 700
#define LINE_4_SCORE 1500

#define LOGO_TEX_SIZE sf::Vector2i(64, 16)
#define LOGO_SIZE sf::Vector2f(256, 64)

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
    
    
    int m_score;
    int m_score_prev;
    int m_lines;
    int m_level;

    sf::Clock m_clk;
    float m_current_delay;
    float m_delay;
    float m_fast_delay;

    void m_genNextDelay();
    void m_resetTetris();

    ////////////////////  ЗВУК  /////////////////////

    Audio m_audio;

    //////////////////// РЕНДЕР /////////////////////

    sf::RenderWindow &m_window;
    sf::Vector2i M_WINDOW_SIZE;
    sf::Vector2i M_TETRIS_GRID_SIZE;

    sf::Vector2i M_SIDE_INTERFACE_SIZE;
    sf::Vector2i M_SIDE_INTERFACE_POS;

    sf::Vector2i M_PREVIEW_SIZE;
    sf::Vector2f M_PREVIEW_CENTER_POS;

    /////////////////// ИНТЕРФЕЙС ///////////////////

    sf::Texture m_logo;

    void m_gameInterface();

    enum StateType
    {
        MENU,
        PAUSE,
        PLAY
    };

    struct M_State
    {
        StateType state = MENU;
    } 
    m_state;
};