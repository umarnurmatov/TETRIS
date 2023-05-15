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
    M_WINDOW_SIZE = wSize;
    M_TETRIS_GRID_SIZE = m_t.setupRender(wSize);
    m_t.setBackground(static_cast<int>(m_state.theme));

    M_SIDE_INTERFACE_SIZE = {wSize.x - M_TETRIS_GRID_SIZE.x, wSize.y * 0.5};
    M_SIDE_INTERFACE_POS = {M_TETRIS_GRID_SIZE.x, 0};

    M_PREVIEW_SIZE = {M_SIDE_INTERFACE_SIZE.x, wSize.y - M_SIDE_INTERFACE_SIZE.y};
    M_PREVIEW_CENTER_POS = sf::Vector2f(M_TETRIS_GRID_SIZE.x + wSize.x, M_SIDE_INTERFACE_SIZE.y + wSize.y) / 2.f;

    sf::IntRect logoArea(sf::Vector2i(0, 0), LOGO_TEX_SIZE);
    m_logo.loadFromFile(Utils::getFilePath("/res/tex/logo.png"), logoArea); 

    m_clk.restart();  
}

void Tetris::processEvent(sf::Event& event)
{
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::A:
            if(m_state.state == PLAY) m_t.moveX(-1);
            break;
        case sf::Keyboard::D:
            if(m_state.state == PLAY) m_t.moveX(1);
            break;
        case sf::Keyboard::W:
            if(m_state.state == PLAY) m_t.rotateCW();
            break;  
        case sf::Keyboard::S:
            if(m_state.state == PLAY) m_t.rotateCCW();
            break;
        case sf::Keyboard::E:
            if(m_state.state == PLAY) m_current_delay = m_fast_delay;
            break;
        case sf::Keyboard::Space:
            if(m_state.state == PLAY) 
            {
                m_t.immediateFall();
                m_audio.putQueue(GAME_SOUNDS::ZAP);
            }
            break;
        case sf::Keyboard::Tab:
            if(m_state.state == PLAY) 
            {
                m_state.state = PAUSE;
                m_audio.pauseMain();
            }
            break;
        case sf::Keyboard::Escape:
            if(m_state.state == PAUSE) 
            {
                m_state.state = PLAY;
                m_audio.playMain();
            }
            break;
        case sf::Keyboard::P:
            if(m_state.state == PLAY) 
            {
                m_resetTetris();
            }
            break;
        case sf::Keyboard::M:
            if(m_state.state == PLAY)
                m_state.state = MENU;
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

    m_t.restart();
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

            if(dLineCount > 0) m_audio.putQueue(HIT);

            if(m_score - m_score_prev >= NEW_LEVEL_SCORE)
            {
                m_score_prev = m_score;
                m_level++;
                m_genNextDelay();
            }
        }
        else
        {
            m_state.state = MENU;
        }
    }
}

void Tetris::render()
{
    m_t.render(m_window);
    if(m_state.state != MENU) m_t.renderPreview(m_window, M_PREVIEW_CENTER_POS);
    m_audio.playQueue();
    m_gameInterface();
}

sf::Color Tetris::getBackgroundColor()
{
    switch (m_state.theme)
    {
    case LIGHT:
        return sf::Color::White;
        break;
    case DARK:
        return sf::Color::Black;
        break;
    default:
        return sf::Color::Magenta;
        break;
    }
}

void Tetris::m_gameInterface()
{
    switch (m_state.theme)
    {
    case LIGHT:
        ImGui::StyleColorsLight();
        break;
    case DARK:
        ImGui::StyleColorsDark();
        break;
    }

    ImGui::Begin("Hi", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowSize(M_SIDE_INTERFACE_SIZE);
    ImGui::SetWindowPos(M_SIDE_INTERFACE_POS);
    ImGui::Text("Lines: %d", m_lines);
    ImGui::Text("Score: %d", m_score);
    ImGui::Text("Level: %d", m_level);
    ImGui::Separator();
    ImGui::Text("[A]/[D]  move right/left");
    ImGui::Text("[W]/[S]  rotate CW/CCW");
    ImGui::Text("[E]      accelerate");
    ImGui::Text("[SPACE]  drop");
    ImGui::Separator();
    ImGui::Text("[TAB]  pause the game");
    ImGui::Text("[P]    restart the game");
    ImGui::Text("[M]    main menu");
    

    if(m_state.state == MENU)
        ImGui::OpenPopup("Menu");

    if (ImGui::BeginPopupModal("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
        sf::Vector2i wSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(M_WINDOW_SIZE / 2 - wSize / 2);

        ImGui::Image(m_logo, LOGO_SIZE);
        ImGui::Text(m_t.isGameEnd() ? "Re[s]tart" : "[S]tart");
        ImGui::Text("S[e]ttings");
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S)) 
        { 

            m_resetTetris();
            m_audio.restartMain();
            m_audio.playMain();

            m_state.state = PLAY; 
            ImGui::CloseCurrentPopup(); 
        }

        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E)) 
        { 
            m_state.state = SETTINGS;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if(m_state.state == SETTINGS)
        ImGui::OpenPopup("Settings");

    if (ImGui::BeginPopupModal("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
        sf::Vector2i wSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(M_WINDOW_SIZE / 2 - wSize / 2);
        
        ImGui::Image(m_logo, LOGO_SIZE);
        ImGui::Text("[R]eturn");
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R)) 
        { 
            m_state.state = MENU; 
            ImGui::CloseCurrentPopup(); 
        }

        int musicType = m_state.music;
        ImGui::Combo("[M]usic", &musicType, "Classic\0Metal\0Piano\0");

        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_M)) 
        { 
            musicType = (musicType + 1) % MAIN_AUDIO_COUNT;
            m_state.music = static_cast<MainMusicType>(musicType);
            m_audio.switchMain(m_state.music);
            m_audio.restartMain();
            m_audio.playMain();
        }

        int themeType = m_state.theme;
        ImGui::Combo("[T]heme", &themeType, "Light\0Dark\0");

        if(ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_T))
        {
            themeType = (themeType + 1) % THEME_COUNT;
            m_state.theme = static_cast<ThemeType>(themeType);
            m_t.setBackground(themeType);
        } 

        ImGui::EndPopup();
    }

    if(m_state.state == PAUSE)
        ImGui::OpenPopup("Pause");
        

    if (ImGui::BeginPopupModal("Pause", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::Text("Game is paused");
        ImGui::Text("[ESCAPE] to continue");
        if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape)) 
        { 
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::EndPopup();
    }
    
    ImGui::End();
}
