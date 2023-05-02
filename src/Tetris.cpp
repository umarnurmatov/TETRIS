#include "Tetris.hpp"

Tetris::Tetris(sf::RenderWindow& window)
    : m_delay{DELAY},
      m_window{window}
{
    m_t.genTetramino();
    m_t.update();
    m_clk.restart();
    m_audio.playMain();
}

void Tetris::processEvent(sf::Event& event)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::A:
            m_t.moveX(-1);
            break;
        case sf::Keyboard::D:
            m_t.moveX(1);
            break;
        case sf::Keyboard::W:
            m_t.rotateCW();
            break;  
        case sf::Keyboard::S:
            m_t.rotateCCW();
            break;
        case sf::Keyboard::E:
            m_delay = DELAY_FAST;
            break;
        case sf::Keyboard::Space:
            immediateFall();
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch(event.key.code)
        {
        case sf::Keyboard::E:
            m_delay = DELAY;
            break;
        }
    }
}

void Tetris::immediateFall()
{
    if(!m_t.isGameEnd())
        while(!m_t.checkBottom())
            m_t.moveY();
}

void Tetris::step()
{
    if(!m_t.isGameEnd())
    {
        if(m_clk.getElapsedTime().asSeconds() >= m_delay)
        {
            m_t.checkBottom();
            m_t.moveY();
            m_clk.restart();
        }
        m_t.update();
    }
}

void Tetris::render()
{
    m_t.render(m_window);
    m_t.renderPreview(m_window, {500, 600});
}