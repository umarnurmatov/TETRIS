#include "Tetris.hpp"

Tetris::Tetris(sf::RenderWindow& window)
    : m_current_delay{DELAY_INIT},
      m_delay{DELAY_INIT},
      m_fast_delay{DELAY_FAST_INIT},
      m_window{window},
      m_score{0},
      m_score_prev{0},
      m_lines{0},
      m_level{0}
{
    m_t.genTetramino();
    sf::Vector2i wSize(window.getSize().x, window.getSize().y);
    M_TETRIS_GRID_SIZE = m_t.setupRender(wSize);

    M_SIDE_INTERFACE_SIZE = {wSize.x - M_TETRIS_GRID_SIZE.x, wSize.y * 0.5};
    M_SIDE_INTERFACE_POS = {M_TETRIS_GRID_SIZE.x, 0};

    M_PREVIEW_SIZE = {M_SIDE_INTERFACE_SIZE.x, wSize.y - M_SIDE_INTERFACE_SIZE.y};
    M_PREVIEW_CENTER_POS = sf::Vector2f(M_TETRIS_GRID_SIZE.x + wSize.x, M_SIDE_INTERFACE_SIZE.y + wSize.y) / 2.f;

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
            m_current_delay = m_fast_delay;
            break;
        case sf::Keyboard::Space:
            m_t.immediateFall();
            m_audio.playSound(GAME_SOUNDS::ZAP);
            break;
        case sf::Keyboard::Escape:
            m_state.state = m_state.state == PLAY ? PAUSE : PLAY;
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch(event.key.code)
        {
        case sf::Keyboard::E:
            m_current_delay = m_delay;
            break;
        }
    }
}

void Tetris::m_genNextDelay()
{
    m_delay -= DELAY_DELTA;
    m_fast_delay -= DELAY_DELTA;
    m_current_delay = m_delay;
}

void Tetris::m_resetTetris()
{
    m_delay = DELAY_INIT;
    m_fast_delay = DELAY_FAST_INIT;
    m_lines = 0;
    m_score = 0;
}

void Tetris::step()
{
    if(m_state.state == PLAY)
    {
        if(!m_t.isGameEnd())
        {
            if(m_clk.getElapsedTime().asMilliseconds() >= m_current_delay)
            {
                m_t.step();
                m_clk.restart();
            }

            int dLineCount = m_t.update();

            m_lines += dLineCount;

            switch(dLineCount)
            {
            case 1:
                m_score += LINE_1_SCORE;
                break;
            case 2:
                m_score += LINE_2_SCORE;
                break;
            case 3:
                m_score += LINE_3_SCORE;
                break;
            case 4:
                m_score += LINE_4_SCORE;
                break;
            }

            if(m_score - m_score_prev >= NEW_LEVEL_SCORE)
            {
                m_score_prev = m_score;
                m_level++;
                m_genNextDelay();
            }
        }
        else
        {
            m_resetTetris();
            m_t.restart();
        }
    }
}

void Tetris::render()
{
    m_t.render(m_window);
    m_t.renderPreview(m_window, M_PREVIEW_CENTER_POS);

    m_gameInterface();
}

void Tetris::m_gameInterface()
{
    ImGui::Begin("Hi", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Lines: %d", m_lines);
    ImGui::Text("Score: %d", m_score);
    ImGui::Text("Level: %d", m_level);
    ImGui::SetWindowSize(M_SIDE_INTERFACE_SIZE);
    ImGui::SetWindowPos(M_SIDE_INTERFACE_POS);

    if(m_state.state == PAUSE)
        ImGui::OpenPopup("Pause");

    if (ImGui::BeginPopupModal("Pause", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("Game is paused");
        ImGui::Text("[ESCAPE] to continue");
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    
    ImGui::End();
}
