#pragma once

#include "SFML/Audio.hpp"
#include <array>
#include <iostream>

#include "Utils.hpp"

#define MAIN_AUDIO_COUNT 3

class Audio
{
    std::array<sf::Music, MAIN_AUDIO_COUNT> mainAudio;
    
    std::array<std::string, MAIN_AUDIO_COUNT> mainAudio_Files = 
    {
        "res/audio/Powerglove-Tetris.flac",
        "res/audio/Scott-Bradlee-Levels-of-Tetris.flac",
        "res/audio/Tetris-Classic-Theme.flac"
    };

    int mainAudio_Current;

public:
    Audio();

    void playMain();
    void pauseMain();

    /// @param num порядковый номер основной темы
    /// @return если переключилось, true
    bool switchMain(int num);
};