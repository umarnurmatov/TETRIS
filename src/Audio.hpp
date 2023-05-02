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

    int mainAudio_Current = 0;

public:
    Audio()
    {
        for(int i = 0; auto& s : mainAudio_Files)
            if(!mainAudio[i].openFromFile(Utils::getFilePath(s)))
                std::cerr << "Failed opening music\n";
            else
                mainAudio[i++].setLoop(true);
    }

    void playMain()
    {
        mainAudio[mainAudio_Current].play();
    }

    void pauseMain()
    {
        mainAudio[mainAudio_Current].pause();
    }
};