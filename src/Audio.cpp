#include "Audio.hpp"

Audio::Audio()
    : m_mainAudio_Current{0}
{
    for(int i = 0; auto& s : m_mainAudio_Files)
    {
        if(!m_mainAudio[i].openFromFile(Utils::getFilePath(s)))
            std::cerr << "Failed opening music " << s << "\n";
        else
            m_mainAudio[i++].setLoop(true);
    }

    for(int i = 0; auto& s : m_gameAudio_Files)
    {
        if(!m_gameAudio_Buffer[i++].loadFromFile(Utils::getFilePath(s)))
            std::cerr << "Failed opening sound " << s << "\n";
    }
    
    for(int i = 0; auto& b : m_gameAudio_Buffer)
    {
        m_gameAudio[i++].setBuffer(b);
    }
}

void Audio::playMain()
{
    m_mainAudio[m_mainAudio_Current].play();
}

void Audio::restartMain()
{
    m_mainAudio[m_mainAudio_Current].setPlayingOffset(sf::seconds(0));
}

void Audio::pauseMain()
{
    m_mainAudio[m_mainAudio_Current].pause();
}

bool Audio::switchMain(MainMusicType t)
{
    m_mainAudio[m_mainAudio_Current].stop();
    m_mainAudio_Current = static_cast<int>(t);
    m_mainAudio[m_mainAudio_Current].play();
    return true;
}

void Audio::playSound(GAME_SOUNDS n)
{
    m_gameAudio[static_cast<int>(n)].play();
}

void Audio::putQueue(GAME_SOUNDS s)
{
    soundQueue.push(s);
}

void Audio::playQueue()
{
    if(!soundQueue.empty())
    {
        playSound(soundQueue.front());
        soundQueue.pop();
    }
}
