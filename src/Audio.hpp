#pragma once

#include "SFML/Audio.hpp"
#include <array>
#include <queue>
#include <iostream>

#include "Utils.hpp"

#define MAIN_AUDIO_COUNT 3
#define GAME_SOUND_COUNT 5

enum GAME_SOUNDS
{
    BEEP,
    BOUNCE,
    HIT,
    PUNCH, 
    ZAP  
};

enum MainMusicType
{
    CLASSIC,
    METAL,
    PIANO
};

class Audio
{
public:
    Audio();

    void playMain();
    void pauseMain();
    void restartMain();

    /// @return если переключилось, true
    bool switchMain(MainMusicType num);

    /// @param num порядковый номер основной темы
    /// @return если переключилось, true
    void playSound(GAME_SOUNDS n);

    std::queue<GAME_SOUNDS> soundQueue;

    void putQueue(GAME_SOUNDS s);
    void playQueue();

private:
    std::array<sf::Music, MAIN_AUDIO_COUNT> m_mainAudio;
    
    std::array<std::string, MAIN_AUDIO_COUNT> m_mainAudio_Files = 
    {
        "res/audio/Tetris-Classic-Theme.flac",
        "res/audio/Powerglove-Tetris.flac",
        "res/audio/Scott-Bradlee-Levels-of-Tetris.flac"
    };

    int m_mainAudio_Current;


    ////////////////////////////////////


    std::array<sf::SoundBuffer, GAME_SOUND_COUNT> m_gameAudio_Buffer;
    std::array<sf::Sound, GAME_SOUND_COUNT> m_gameAudio;

    std::array<std::string, GAME_SOUND_COUNT> m_gameAudio_Files = 
    {
        "res/audio/Beep.flac",
        "res/audio/Bounce.flac",
        "res/audio/Hit.flac",
        "res/audio/Punch.flac",
        "res/audio/Zap.flac"
    };

};