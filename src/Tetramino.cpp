#include "Tetramino.hpp"

Tetramino::Tetramino()
    : uid{0, 6}
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rd.seed(seed);

    
    getNext();
    clearGridAll();
    setupRenderGrid();
}

void Tetramino::getNext()
{
    next = rd()%7;
    next_col = rd()%7;
}

void Tetramino::setupRenderGrid()
{
    for(size_t i = 0; i < GRID_H*GRID_W; i++)
    {
        render_grid[i].setSize(sf::Vector2f(SQUARE_A-OUTLINE, SQUARE_A-OUTLINE));
        render_grid[i].setOutlineThickness(OUTLINE);
        render_grid[i].setOutlineColor(OUTLINE_COLOR);
        render_grid[i].setPosition(sf::Vector2f(SQUARE_A*(i % GRID_W)+OUTLINE/2.f, SQUARE_A*floor(i / GRID_W)+OUTLINE/2.f));
    }
}

void Tetramino::clearGrid()
{
    for(auto& s : grid)
        if(s.type != SquareType::FALLEN)
        {
            s.color = EMPTY_COLOR;
            s.type = SquareType::EMPTY;
        }
}

void Tetramino::clearGridAll()
{
    for(auto& s : grid)
    {
        s.color = EMPTY_COLOR;
        s.type = SquareType::EMPTY;
    }
}

void Tetramino::genTetramino()
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

void Tetramino::moveX(int x)
{
    tpos.x += x;
    if(checkCollision()) tpos.x -= x;
}

void Tetramino::moveY()
{
    tpos.y += 1;
}

void Tetramino::rotateCW() 
{ 
    if(current == 6) return; // если тетраминошка - квадрат
    for(auto& v : t) 
        rotateCW_90(v);

    if(checkCollision())
        for(auto& v : t) 
            rotateCCW_90(v);
}
void Tetramino::rotateCCW() 
{ 
    if(current == 6) return; // если тетраминошка - квадрат
    for(auto& v : t) 
        rotateCCW_90(v); 

    if(checkCollision())
        for(auto& v : t) 
            rotateCW_90(v);
}

bool Tetramino::checkCollision()
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

bool Tetramino::checkBottom()
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

    return bottom;
}

int Tetramino::checkLine()
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

void Tetramino::update()
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

void Tetramino::render(sf::RenderWindow &w)
{
    for(size_t i = 0; i < grid.size(); i++)
    {
        render_grid[i].setFillColor(grid[i].color);
        w.draw(render_grid[i]);
    }
}