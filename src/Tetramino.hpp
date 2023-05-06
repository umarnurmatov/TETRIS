#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include <random>
#include <array>
#include <chrono>

#include "Utils.hpp"

#define GRID_H 20
#define GRID_W 10

#define TETRAMINO_W 2
#define TETRAMINO_H 4

#define SQUARE_PER_TETRAMINO 4
#define TETRAMINO_COUNT 7

#define SQUARE_TEX_COUNT 9
#define BACKGROUND_SQUARE_TEX_COUNT 2
#define WHITE_BACK 10
#define BLACK_BACK 9

#define OUTLINE 1

enum SquareType
{
    EMPTY,  // пустая клеточка
    FALLING,// падающая тетраминошка
    FALLEN  // упавшая тетраминошка
};

struct Square
{
    sf::IntRect rect;
    SquareType type;
};

class Tetramino
{   
public:
    
    Tetramino();

    //////////////////// ЛОГИКА //////////////////// 

    void genTetramino();

    /// @brief двигает тетрамино по оси x
    void moveX(int x);

    void rotateCW();
    void rotateCCW();

    /// @brief тетрамино сразу падает вниз
    void immediateFall();

    bool isGameEnd() { return m_isGameEnd; }

    /// @brief двигает тетрамино вниз
    void step();

    /// @brief обновляет сетку (после moveX, rotateCW/CCW надо вызвать, чтобы применить изменения)
    ///        проверяет линии
    /// @return количество удаленных линий
    int update();

    void restart();

    //////////////////// РЕНДЕР //////////////////// 

    /// @param renderGridSize размер области для отрисовки
    /// @param fitCoord т.к. квадратики тетрмино должны быть квадратные(!), их размер будет вычисляться относительно 
    ///                 x или y размера grid. 0 = x, 1 = y
    /// @param previewSquareSize размер квадратика на превью; по умолчанию такой же как и на игровом поле
    /// @return получившиеся размеры сетки
    sf::Vector2i setupRender(sf::Vector2i renderGridSize, int fitCoord = 1, sf::Vector2f previewSquareSize = {});


    void render(sf::RenderWindow &w);

    /// @brief рендерит превью следующего тетрамино
    /// @param position позиция центра превью тетрамино на экране
    void renderPreview(sf::RenderWindow &w, sf::Vector2f position);

private:
    //////////////////// ЛОГИКА ////////////////////

    sf::Vector2i m_tpos;
    std::array<Square, GRID_W*GRID_H> m_grid;

    int m_current;
    int m_next;
    
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

    bool m_isGameEnd;

    /// @brief grid[i] = EMPTY if grid[i] is not FALLEN
    void m_clearGrid();

    /// @brief grid[i] = EMPTY
    void m_clearGridAll();

    /// @brief проверяет не упала ли тетрамино на дно / другие тетрамино
    ///        если упала, то генерирует новую тетрамино
    /// @return true если упала / false если нет
    bool m_checkBottom();

    /// @brief проверяет не заполнена ли какая-то линия; сразу удаляет заполненную
    /// @return количество удаленных линий
    int m_checkLine();

    void m_moveY();

    //////////////////// РЕНДЕР ////////////////////

    float M_SQUARE_A;
    int M_WINDOW_H;
    int M_WINDOW_W;
    float M_OUTLINE;    

    sf::Texture m_square_texture;
    std::vector<sf::IntRect> m_squares
    {
        {
            {sf::Vector2i{0, 0}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 10}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 20}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 30}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 40}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 50}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 60}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 70}, sf::Vector2i{10, 10}},
            {sf::Vector2i{0, 80}, sf::Vector2i{10, 10}},

            {sf::Vector2i{10, 0}, sf::Vector2i{20, 20}},  // Black background
            {sf::Vector2i{10, 20}, sf::Vector2i{20, 20}}  // White background
        }  
    };

    std::array<sf::RectangleShape, GRID_H*GRID_H> m_render_grid;
    std::array<sf::RectangleShape, SQUARE_PER_TETRAMINO> m_render_preview;

    int m_current_square; // [0, SQUARE_TEX_COUNT)
    int m_next_square;    // [0, SQUARE_TEX_COUNT)

    void m_setupRenderGrid();
    void m_setupRenderPreview(sf::Vector2f squareSize = {});

};