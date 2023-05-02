#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include <random>
#include <array>
#include <chrono>

#define GRID_H 20
#define GRID_W 10

#define WINDOW_H 800
#define WINDOW_W WINDOW_H * GRID_W / GRID_H

#define SQUARE_A WINDOW_H / GRID_H
#define OUTLINE 5.f

#define TETRAMINO_W 2
#define TETRAMINO_H 4

#define EMPTY_COLOR sf::Color::White
#define OUTLINE_COLOR sf::Color::White

#define SQUARE_PER_TETRAMINO 4
#define TETRAMINO_COUNT 7

using std::cout, std::endl;

enum SquareType
{
    EMPTY,  // пустая клеточка
    FALLING,// падающая тетраминошка
    FALLEN  // упавшая тетраминошка
};

struct Square
{
    sf::Color color;
    SquareType type;
};

class Tetramino
{   
public:
    Tetramino();

    //////////////////// ЛОГИКА //////////////////// 

    /// @brief grid[i] = EMPTY if grid[i] is not FALLEN
    void clearGrid();

    /// @brief grid[i] = EMPTY
    void clearGridAll();

    void genTetramino();

    void moveX(int x);
    void moveY();

    void rotateCW();
    void rotateCCW();    

    /// @brief проверяет не упала ли тетрамино на дно / другие тетрамино
    ///        если упала, то генерирует новую тетрамино
    /// @return true если упала / false если нет
    bool checkBottom();

    /// @brief проверяет не заполнена ли какая-то линия; сразу удаляет заполненную
    /// @return количество удаленных линий
    int checkLine();

    void update();

    //////////////////// РЕНДЕР //////////////////// 

    void render(sf::RenderWindow &w);  

private:
    //////////////////// ЛОГИКА ////////////////////

    sf::Vector2i m_tpos;
    std::array<Square, GRID_W*GRID_H> m_grid;

    int m_current;
    int m_next;
    int m_current_col;
    int m_next_col;
    
    /* -----
       |0|1|
       -----
       |2|3|
       ----- */
    std::array<std::array<int, SQUARE_PER_TETRAMINO>, TETRAMINO_COUNT> m_tetramino
    { 
        { {1,3,5,7}, {2,4,5,7}, {3,4,5,6}, {3,4,5,7}, {2,3,5,7}, {3,5,6,7}, {2,3,4,5} } 
    };

    // xi, yi - центр вращения для i tetramino
    std::array<std::array<int, 2>, TETRAMINO_COUNT> m_centers
    {
        { {1,1}, {0,2}, {0,2}, {1,2}, {1,2}, {1,2}, {1,2} }
    };

    std::array<sf::Color, TETRAMINO_COUNT> m_colors
    {
        { sf::Color::Green, sf::Color::Blue, sf::Color::Red, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow, {121, 240, 15} }
    };

    int m_XYtoSerial(int x, int y) { return y * GRID_W + x; }

    std::array<sf::Vector2i, SQUARE_PER_TETRAMINO> m_t, m_t_next;
    void m_rotateCW_90(sf::Vector2i& v) { v = sf::Vector2i(v.y, -v.x); }
    void m_rotateCCW_90(sf::Vector2i& v) { v = sf::Vector2i(-v.y, v.x); }

    void m_getNext();
    void m_initTetramino(std::array<sf::Vector2i, SQUARE_PER_TETRAMINO>& t, int& number);

    std::minstd_rand m_rd;
    std::uniform_int_distribution<> m_uid;
    int m_rnd_not_same_as_x(int x) { int temp = x; while(temp==x) temp = m_rd()%TETRAMINO_COUNT; return temp; }

    bool m_checkCollision();

    //////////////////// РЕНДЕР ////////////////////

    std::array<sf::RectangleShape, GRID_H*GRID_H> m_render_grid;
    std::array<sf::RectangleShape, SQUARE_PER_TETRAMINO> m_render_preview;

    void m_setupRenderGrid();
};