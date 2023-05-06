#include "Tetramino.hpp"

Tetramino::Tetramino()
    : m_uid{0, 6},
      m_isGameEnd{false}
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_rd.seed(seed);

    m_square_texture.loadFromFile(Utils::getFilePath("res/tex/squares.png"));

    m_getNext();
    m_initTetramino(m_t_next, m_next);
    m_clearGridAll();
}

void Tetramino::m_getNext()
{
    m_next = m_rd()%TETRAMINO_COUNT;
    m_next_square = m_rd()%SQUARE_TEX_COUNT;
}

void Tetramino::m_clearGrid()
{
    for(auto& s : m_grid)
        if(s.type != SquareType::FALLEN)
        {
            s.rect = m_squares[WHITE_BACK];
            s.type = SquareType::EMPTY;
        }
}

void Tetramino::m_clearGridAll()
{
    for(auto& s : m_grid)
    {
        s.rect = m_squares[WHITE_BACK];
        s.type = SquareType::EMPTY;
    }
}

void Tetramino::genTetramino()
{
    m_tpos = {GRID_W / 2, TETRAMINO_H / 2 - 1};

    m_current_square = m_next_square;
    m_current = m_next;
    m_t = m_t_next;

    m_getNext();
    m_initTetramino(m_t_next, m_next);

    if(m_checkCollision()) m_isGameEnd = true;
}

void Tetramino::m_initTetramino(std::array<sf::Vector2i, 4>& m_t, int& number)
{
    sf::Vector2i center(m_centers[number][0], m_centers[number][1]);
    for(int i = 0; int& n : m_tetramino[number])
    {
        sf::Vector2i p(n % TETRAMINO_W, n / TETRAMINO_W);
        m_t[i++] = p - center;
    }
}

void Tetramino::moveX(int x)
{
    m_tpos.x += x;
    if(m_checkCollision()) m_tpos.x -= x;
}

void Tetramino::m_moveY()
{
    m_tpos.y += 1;
}

void Tetramino::rotateCW() 
{ 
    if(m_current == 6) return; // если тетраминошка - квадрат
    for(auto& v : m_t) 
        m_rotateCW_90(v);

    if(m_checkCollision())
        for(auto& v : m_t) 
            m_rotateCCW_90(v);
}
void Tetramino::rotateCCW() 
{ 
    if(m_current == 6) return; // если тетраминошка - квадрат
    for(auto& v : m_t) 
        m_rotateCCW_90(v); 

    if(m_checkCollision())
        for(auto& v : m_t) 
            m_rotateCW_90(v);
}

void Tetramino::immediateFall()
{
    if(!m_isGameEnd)
        while(!m_checkBottom())
            m_moveY();
}

bool Tetramino::m_checkCollision()
{
    for(auto& v : m_t)
    {
        sf::Vector2i p = v + m_tpos;
        if(p.x < 0 || p.x > GRID_W-1 || p.y<0 || p.y>GRID_H-1 ||
           m_grid[m_XYtoSerial(p.x, p.y)].type == FALLEN)

            return true;
    }
    return false;
}

bool Tetramino::m_checkBottom()
{
    bool bottom = false;
    for(auto& v : m_t)
    {
        sf::Vector2i p = v + m_tpos;
        if(p.y == GRID_H-1 || m_grid[m_XYtoSerial(p.x, p.y+1)].type == SquareType::FALLEN)
        {
            bottom = true;
            break;
        }
    }

    if(bottom)
    {
        for(auto& v : m_t)
        {   
            sf::Vector2i p = v + m_tpos;
            m_grid[m_XYtoSerial(p.x, p.y)].type = SquareType::FALLEN;
            m_grid[m_XYtoSerial(p.x, p.y)].rect = m_squares[m_current_square];
        }

        genTetramino();
    }

    return bottom;
}

int Tetramino::m_checkLine()
{
    bool is_line;
    int line_count = 0;
    for(int y = 0; y < GRID_H; y++)
    {
        is_line = true;
        for(int x = 0; x < GRID_W; x++)
            if(m_grid[m_XYtoSerial(x, y)].type == SquareType::EMPTY || m_grid[m_XYtoSerial(x, y)].type == SquareType::FALLING)
                is_line = false;
        
        if(is_line)
        {
            line_count++;
            for(int yy = y; yy > 0; yy--)
                for(int xx = 0; xx < GRID_W; xx++)
                {
                    m_grid[m_XYtoSerial(xx, yy)].rect = m_grid[m_XYtoSerial(xx, yy-1)].rect;
                    if(m_grid[m_XYtoSerial(xx, yy-1)].type != SquareType::FALLING)
                        m_grid[m_XYtoSerial(xx, yy)].type = m_grid[m_XYtoSerial(xx, yy-1)].type;
                }
        }
    }

    return line_count;
}

void Tetramino::step()
{
    if(m_isGameEnd) return;

    m_checkBottom();
    m_moveY();
}

int Tetramino::update()
{
    if(m_isGameEnd) return 0;

    int lineCount = m_checkLine();

    m_clearGrid();

    for(auto& v : m_t)
    {   
        sf::Vector2i p = v + m_tpos;
        m_grid[m_XYtoSerial(p.x, p.y)].type = SquareType::FALLING;
        m_grid[m_XYtoSerial(p.x, p.y)].rect = m_squares[m_current_square];
    }

    return lineCount;
}

void Tetramino::restart()
{
    m_isGameEnd = false;
    m_clearGridAll();
    genTetramino();
}

void Tetramino::m_setupRenderGrid()
{
    for(int i = 0; i < GRID_H*GRID_W; i++)
    {
        m_render_grid[i].setSize(sf::Vector2f(M_SQUARE_A-OUTLINE, M_SQUARE_A-OUTLINE));
        m_render_grid[i].setOrigin(sf::Vector2f(-OUTLINE, -OUTLINE));
        m_render_grid[i].setTexture(&m_square_texture);
        m_render_grid[i].setTextureRect(m_squares[m_current_square]);
        m_render_grid[i].setPosition(sf::Vector2f(M_SQUARE_A*(i % GRID_W), M_SQUARE_A*floor(i / GRID_W)));
    }

}

void Tetramino::m_setupRenderPreview(sf::Vector2f squareSize)
{
    if(squareSize == sf::Vector2f()) squareSize = {M_SQUARE_A, M_SQUARE_A};
    for(int i = 0; i < SQUARE_PER_TETRAMINO; i++)
    {
        m_render_preview[i].setSize(squareSize - sf::Vector2f{OUTLINE, OUTLINE});
        m_render_grid[i].setOrigin(sf::Vector2f(-OUTLINE, -OUTLINE));
        m_render_preview[i].setTexture(&m_square_texture);
        m_render_preview[i].setTextureRect(m_squares[m_next_square]);
    }
}

sf::Vector2i Tetramino::setupRender(sf::Vector2i renderGridSize, int fitCoord, sf::Vector2f previewSquareSize)
{
    assert(fitCoord == 0 || fitCoord == 1);
    if(fitCoord == 0)
    {
        M_WINDOW_W = renderGridSize.x;
        M_WINDOW_H = M_WINDOW_W * GRID_H / GRID_W;
        M_SQUARE_A = M_WINDOW_W / GRID_W;
    }
    else
    {
        M_WINDOW_H = renderGridSize.y;
        M_WINDOW_W = M_WINDOW_H * GRID_W / GRID_H;
        M_SQUARE_A = M_WINDOW_H / GRID_H;
    }

    m_setupRenderGrid();
    m_setupRenderPreview(previewSquareSize);

    return sf::Vector2i(M_WINDOW_W, M_WINDOW_H);
}

void Tetramino::render(sf::RenderWindow &w)
{
    for(size_t i = 0; i < m_grid.size(); i++)
    {
        m_render_grid[i].setTextureRect(m_grid[i].rect);
        w.draw(m_render_grid[i]);
    }
}

void Tetramino::renderPreview(sf::RenderWindow &w, sf::Vector2f position)
{
    sf::Vector2f center{m_centers[m_next][0], m_centers[m_next][1]};
    static sf::Vector2f popravka(sf::Vector2f(M_SQUARE_A * TETRAMINO_W / 2.f, M_SQUARE_A * TETRAMINO_H / 2.f));
    for(int i = 0; i < SQUARE_PER_TETRAMINO; i++)
    {
        m_render_preview[i].setTextureRect(m_squares[m_next_square]);
        m_render_preview[i].setPosition(position + (center + sf::Vector2f{m_t_next[i].x, m_t_next[i].y})*M_SQUARE_A - popravka);
    }

    for(auto& r : m_render_preview)
        w.draw(r);
}
