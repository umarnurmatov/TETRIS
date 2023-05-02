#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "bits/stdc++.h"


#define GRID_H 20
#define GRID_W 10

#define WINDOW_H 800
#define WINDOW_W WINDOW_H * GRID_W / GRID_H

#define SQUARE_A WINDOW_H / GRID_H
#define OUTLINE 5.f

#define TETRAMINO_W 2
#define TETRAMINO_H 4

#define DELAY 0.5f
#define DELAY_FAST 0.1f

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

    // xi, yi - центр вращения для i тетрамино
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

    //////////
    std::array<sf::RectangleShape, GRID_H*GRID_H> render_grid;
    //////////
    
public:
    Tetramino()
        : uid{0, 6}
    {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        rd.seed(seed);

        
        getNext();
        clearGridAll();
        setupRenderGrid();
    }

    void getNext()
    {
        next = rd()%7;
        next_col = rd()%7;
    }

    void setupRenderGrid()
    {
        for(size_t i = 0; i < GRID_H*GRID_W; i++)
        {
            render_grid[i].setSize(sf::Vector2f(SQUARE_A-OUTLINE, SQUARE_A-OUTLINE));
            render_grid[i].setOutlineThickness(OUTLINE);
            render_grid[i].setOutlineColor(OUTLINE_COLOR);
            render_grid[i].setPosition(sf::Vector2f(SQUARE_A*(i % GRID_W)+OUTLINE/2.f, SQUARE_A*floor(i / GRID_W)+OUTLINE/2.f));
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
        tpos = {GRID_W / 2, TETRAMINO_H / 2};

        current_col = next_col;
        current = next;

        sf::Vector2i center(centers[current][0], centers[current][1]);
        for(int i = 0; int& n : tetramino[current])
        {
            sf::Vector2i p(n % TETRAMINO_W, n / TETRAMINO_W);
            t[i++] = p - center;
        }

        getNext();
    }

    void moveX(int x)
    {
        tpos.x += x;
        if(checkCollision()) tpos.x -= x;
    }

    void moveY()
    {
        tpos.y += 1;
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
            if(p.x < 0 || p.x > GRID_W-1 || p.y<0 || p.y>GRID_H-1 ||
                grid[XYtoSerial(p.x, p.y)].type == FALLEN)

                return true;
        }
        return false;
    }

    void checkBottom()
    {
        bool bottom = false;
        for(auto& v : t)
        {
            sf::Vector2i p = v + tpos;
            if(p.y == GRID_H-1 || grid[XYtoSerial(p.x, p.y+1)].type == SquareType::FALLEN)
                {
                    bottom = true;
                    break;
                }
        }

        if(bottom)
        {
            for(auto& v : t)
            {   
                sf::Vector2i p = v + tpos;
                grid[XYtoSerial(p.x, p.y)].type = SquareType::FALLEN;
                grid[XYtoSerial(p.x, p.y)].color = colors[current_col];
            }

            genTetramino();
        }
    }

    int checkLine()
    {
        bool is_line;
        int line_count = 0;
        for(int y = 0; y < GRID_H; y++)
        {
            is_line = true;
            for(int x = 0; x < GRID_W; x++)
                if(grid[XYtoSerial(x, y)].type == SquareType::EMPTY || grid[XYtoSerial(x, y)].type == SquareType::FALLING)
                    is_line = false;
            
            if(is_line)
            {
                line_count++;
                for(int yy = y; yy > 0; yy--)
                    for(int xx = 0; xx < GRID_W; xx++)
                    {
                        grid[XYtoSerial(xx, yy)].color = grid[XYtoSerial(xx, yy-1)].color;
                        if(grid[XYtoSerial(xx, yy-1)].type != SquareType::FALLING)
                            grid[XYtoSerial(xx, yy)].type = grid[XYtoSerial(xx, yy-1)].type;
                    }
            }
        }

        return line_count;
    }

    void update()
    {
        checkLine();

        clearGrid();

        for(auto& v : t)
        {   
            sf::Vector2i p = v + tpos;
            grid[XYtoSerial(p.x, p.y)].type = SquareType::FALLING;
            grid[XYtoSerial(p.x, p.y)].color = colors[current_col];
        }

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
    sf::Clock clk;
    float delay;

public:
    Tetris()
        : delay{DELAY}
    {
        t.genTetramino();
        t.update();
        clk.restart();
    }

    void processEvent(sf::Event& event)
    {
        switch(event.type)
        {
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
            case sf::Keyboard::A:
                t.moveX(-1);
                break;
            case sf::Keyboard::D:
                t.moveX(1);
                break;
            case sf::Keyboard::W:
                t.rotateCW();
                break;  
            case sf::Keyboard::S:
                t.rotateCCW();
                break;
            case sf::Keyboard::E:
                delay = DELAY_FAST;
                break;
            }
            break;
        case sf::Event::KeyReleased:
            switch(event.key.code)
            {
            case sf::Keyboard::E:
                delay = DELAY;
                break;
            }
        }
    }

    void render(sf::RenderWindow &window)
    {
        if(clk.getElapsedTime().asSeconds() >= delay)
        {
            t.checkBottom();
            t.moveY();
            clk.restart();
        }
        t.update();
        t.render(window);
    }
};

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::VideoMode vm(sf::Vector2u(WINDOW_W+400, WINDOW_H));
    sf::RenderWindow window(vm, "Tetris", sf::Style::Default ^ sf::Style::Resize, settings);
    window.setFramerateLimit(60);
    bool imgui_sfml_init = ImGui::SFML::Init(window);
    sf::View view;

    Tetris t; 

    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Q && event.key.control)) {
                window.close();
            }

            if(event.type == sf::Event::Resized)
            {
                view = window.getDefaultView();
                view.setSize({event.size.width, event.size.height});
                window.setView(view);
            }

            t.processEvent(event);
            //sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hi", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Hi");
        ImGui::SetWindowSize(ImVec2(400, WINDOW_H/2));
        ImGui::SetWindowPos(ImVec2(WINDOW_W + OUTLINE, 0));
        
        ImGui::End();
        

        window.clear();
        t.render(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}