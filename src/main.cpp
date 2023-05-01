#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "bits/stdc++.h"


#define GRID_H 20
#define GRID_W 10

#define WINDOW_H 600
#define WINDOW_W WINDOW_H * GRID_W / GRID_H

#define SQUARE_A WINDOW_H / GRID_H

#define TETRAMINO_W 2
#define TETRAMINO_H 4

#define EMPTY_COLOR sf::Color::Black

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

    std::array<std::array<int, 2>, 7> centers
    {
        { {1,1}, {0,2}, {0,2}, {1,2}, {1,2}, {1,2}, {1,2} }
    };

    std::array<sf::Color, 7> colors
    {
        { sf::Color::Green, sf::Color::Blue, sf::Color::Red, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow, {121, 240, 15} }
    };

    std::array<sf::Vector2i, 4> t;
    void rotateCW_90(sf::Vector2i& v) { v = sf::Vector2i(v.y, -v.x); }
    void rotateCCW_90(sf::Vector2i& v) { v = sf::Vector2i(-v.y, v.x); }

    std::minstd_rand rd;
    std::uniform_int_distribution<> uid;

    //////////
    std::array<sf::RectangleShape, GRID_H*GRID_H> render_grid;

    
public:
    Tetramino()
        : uid(0, 6),
          tpos(GRID_W / 2, TETRAMINO_H / 2)
    {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        rd.seed(seed);

        next = uid(rd);
        next_col = uid(rd);

        clearGridAll();
        setupRenderGrid();
        
    }

    void setupRenderGrid()
    {
        for(size_t i = 0; i < GRID_H*GRID_W; i++)
        {
            render_grid[i].setSize(sf::Vector2f(SQUARE_A - 5, SQUARE_A - 5));
            render_grid[i].setPosition(sf::Vector2f(SQUARE_A*(i % GRID_W), SQUARE_A*floor(i / GRID_W)));
        }
    }

    void clearGrid()
    {
        for(auto& s : grid)
            if(s.type != SquareType::FALLEN)
            {
                s.color = EMPTY_COLOR;
                s.type = SquareType::EMPTY;
            }
    }

    void clearGridAll()
    {
        for(auto& s : grid)
        {
            s.color = EMPTY_COLOR;
            s.type = SquareType::EMPTY;
        }
    }

    void genTetramino()
    {
        current_col = next_col;
        current = next;

        sf::Vector2i center(centers[current][0], centers[current][1]);
        for(int i = 0; int& n : tetramino[current])
        {
            sf::Vector2i p(n % TETRAMINO_W, n / TETRAMINO_W);
            t[i++] = p - center;
        }

        next = uid(rd);
        next_col = uid(rd);
    }

    void update()
    {
        //tpos.y += 1;
        clearGrid();
        for(auto& v : t)
        {   
            sf::Vector2i p = v + tpos;
            grid[p.y * GRID_W + p.x].type = SquareType::FALLING;
            grid[p.y * GRID_W + p.x].color = colors[current_col];
        }
    }

    void move(int x)
    {
        tpos.x += x;
        if(checkCollision()) tpos.x -= x;
    }

    void rotateCW() 
    { 
        if(current == 6) return; // если тетраминошка - квадрат
        for(auto& v : t) 
            rotateCW_90(v);

        if(checkCollision())
            for(auto& v : t) 
                rotateCCW_90(v);
    }
    void rotateCCW() 
    { 
        if(current == 6) return; // если тетраминошка - квадрат
        for(auto& v : t) 
            rotateCCW_90(v); 

        if(checkCollision())
            for(auto& v : t) 
                rotateCW_90(v);
    }

    bool checkCollision()
    {
        for(auto& v : t)
        {
            sf::Vector2i p = v + tpos;
            if(p.x < 0 || p.x > GRID_W-1 || p.y<0 || p.y>GRID_H-1)
                return true;
        }
        return false;
    }

    void render(sf::RenderWindow &w)
    {
        for(size_t i = 0; i < grid.size(); i++)
        {
            render_grid[i].setFillColor(grid[i].color);
            w.draw(render_grid[i]);
        }
    }       
};

class Tetris
{
    Tetramino t;
public:
    Tetris()
    {
        t.genTetramino();
        t.update();
    }

    void processEvent(sf::Event& event)
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
            case sf::Keyboard::A:
                t.move(-1);
                break;
            case sf::Keyboard::D:
                t.move(1);
                break;
            case sf::Keyboard::W:
                t.rotateCW();
                break;  
            case sf::Keyboard::S:
                t.rotateCCW();
                break;  
            }
            break;
        }
    }

    void render(sf::RenderWindow &window)
    {
        t.update();
        t.render(window);
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_W, WINDOW_H)), "Tetris", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    bool imgui_sfml_init = ImGui::SFML::Init(window);

    Tetris t; 

    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Q && event.key.control)) {
                window.close();
            }

            t.processEvent(event);
            //sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        

        window.clear();
        t.render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}