#include "Audio.hpp"

Audio::Audio()
    : m_mainAudio_Current{0}
{
    for(int i = 0; auto& s : m_mainAudio_Files)
        if(!m_mainAudio[i].openFromFile(Utils::getFilePath(s)))
            std::cerr << "Failed opening music\n";
        else
            m_mainAudio[i++].setLoop(true);
}

void Audio::playMain()
{
    m_mainAudio[m_mainAudio_Current].play();
}

void Audio::pauseMain()
{
    m_mainAudio[m_mainAudio_Current].pause();
}

/// @param num порядковый номер основной темы
/// @return если переключилось, true
bool Audio::switchMain(int num)
{
    if(num > MAIN_AUDIO_COUNT)
        return false;
    else
    {
        m_mainAudio[m_mainAudio_Current].stop();
        m_mainAudio_Current = num;
        m_mainAudio[m_mainAudio_Current].play();
        return true;
    }
}