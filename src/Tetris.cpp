#include "Tetris.h"

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

void Tetris::render(sf::RenderWindow &window)
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