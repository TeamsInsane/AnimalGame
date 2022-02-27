//
// Created by Uporabnik on 27. 02. 2022.
//

#ifndef ANIMALGAME_SOUNDMANAGER_H
#define ANIMALGAME_SOUNDMANAGER_H


#include <map>
#include <string>
#include <SDL_mixer.h>
#include <SDL_log.h>
#include "../Vendor/tinyxml.h"

#define FREQ 44100
#define CHUNK_SIZE 2048

using EffectMap = std::map<std::string, Mix_Chunk*>;
using MusicMap = std::map<std::string, Mix_Music*>;

class SoundManager {
private:
    SoundManager();
    MusicMap musicMap;
    EffectMap effectMap;
    static SoundManager *instance;
public:
    void clean();

    void playMusic(std::string id);
    void loadMusic(std::string id, std::string source);

    void playEffect(std::string id);
    void loadEffect(std::string id, std::string source);

    bool parseSounds(std::string source);

    static SoundManager *getInstance();
};


#endif //ANIMALGAME_SOUNDMANAGER_H
