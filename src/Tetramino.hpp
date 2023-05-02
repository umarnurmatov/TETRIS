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

    void getNext();

    void setupRenderGrid();

    /// @brief grid[i] = EMPTY if grid[i] is not FALLEN
    void clearGrid();

    /// @brief grid[i] = EMPTY
    void clearGridAll();

    void genTetramino();

    void moveX(int x);
    void moveY();

    void rotateCW();
    void rotateCCW();

    bool checkCollision();

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

    sf::Vector2i tpos;
    std::array<Square, GRID_W*GRID_H> grid;

    int current;
    int next;
    int current_col;
    int next_col;
    
    /* -----
       |0|1|
       -----
       |2|3|
       ----- */
    std::array<std::array<int, 4>, 7> tetramino
    { 
        { {1,3,5,7}, {2,4,5,7}, {3,4,5,6}, {3,4,5,7}, {2,3,5,7}, {3,5,6,7}, {2,3,4,5} } 
    };

    // xi, yi - центр вращения для i tetramino
    std::array<std::array<int, 2>, 7> centers
    {
        { {1,1}, {0,2}, {0,2}, {1,2}, {1,2}, {1,2}, {1,2} }
    };

    std::array<sf::Color, 7> colors
    {
        { sf::Color::Green, sf::Color::Blue, sf::Color::Red, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow, {121, 240, 15} }
    };

    int XYtoSerial(int x, int y) { return y * GRID_W + x; }

    std::array<sf::Vector2i, 4> t;
    void rotateCW_90(sf::Vector2i& v) { v = sf::Vector2i(v.y, -v.x); }
    void rotateCCW_90(sf::Vector2i& v) { v = sf::Vector2i(-v.y, v.x); }

    std::minstd_rand rd;
    std::uniform_int_distribution<> uid;
    int rnd_not_same_as_x(int x) { int temp = x; while(temp==x) temp = rd()%7; return temp; }

    //////////////////// РЕНДЕР ////////////////////

    std::array<sf::RectangleShape, GRID_H*GRID_H> render_grid;
    
};