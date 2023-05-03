#include "Tetris.hpp"

Tetris::Tetris(sf::RenderWindow& window)
    : m_delay{DELAY},
      m_window{window},
      score{0},
      lines{0}
{
    m_t.genTetramino();
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
            m_t.immediateFall();
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

void Tetris::step()
{
    if(!m_t.isGameEnd())
    {
        if(m_clk.getElapsedTime().asSeconds() >= m_delay)
        {
            m_t.step();
            m_clk.restart();
        }
        int dLineCount;
        m_t.update(dLineCount);
        lines += dLineCount;
    }
}

void Tetris::render()
{
    m_t.render(m_window);
    m_t.renderPreview(m_window, {500, 600});
    m_showGameInterface();
}

void Tetris::m_showGameInterface()
{
    ImGui::Begin("Hi", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Lines: %d", lines);
    ImGui::SetWindowSize(ImVec2(400, WINDOW_H/2));
    ImGui::SetWindowPos(ImVec2(WINDOW_W + OUTLINE, 0));
    
    ImGui::End();
}
