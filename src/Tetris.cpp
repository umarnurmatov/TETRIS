#include "Tetris.hpp"

Tetris::Tetris()
    : delay{DELAY}
{
    t.genTetramino();
    t.update();
    clk.restart();
    audio.playMain();
}

void Tetris::processEvent(sf::Event& event)
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
        case sf::Keyboard::Space:
            immediateFall();
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

void Tetris::immediateFall()
{
    while(!t.checkBottom())
        t.moveY();
}

void Tetris::step()
{
    if(clk.getElapsedTime().asSeconds() >= delay)
    {
        t.checkBottom();
        t.moveY();
        clk.restart();
    }
    t.update();
}

void Tetris::render(sf::RenderWindow &window)
{
    t.render(window);
}