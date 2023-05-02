#include "Audio.hpp"

Audio::Audio()
    : mainAudio_Current{0}
{
    for(int i = 0; auto& s : mainAudio_Files)
        if(!mainAudio[i].openFromFile(Utils::getFilePath(s)))
            std::cerr << "Failed opening music\n";
        else
            mainAudio[i++].setLoop(true);
}

void Audio::playMain()
{
    mainAudio[mainAudio_Current].play();
}

void Audio::pauseMain()
{
    mainAudio[mainAudio_Current].pause();
}

/// @param num порядковый номер основной темы
/// @return если переключилось, true
bool Audio::switchMain(int num)
{
    if(num > MAIN_AUDIO_COUNT)
        return false;
    else
    {
        mainAudio[mainAudio_Current].stop();
        mainAudio_Current = num;
        mainAudio[mainAudio_Current].play();
        return true;
    }
}