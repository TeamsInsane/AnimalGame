//
// Created by TeamsInsane on 27. 02. 2022.
//

#ifndef ANIMALGAME_SOUNDMANAGER_H
#define ANIMALGAME_SOUNDMANAGER_H


#include <map>
#include <string>
#include <SDL_mixer.h>
#include <SDL_log.h>
#include "../Vendor/tinyxml.h"
#include "../Characters/Warrior.h"

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
    bool music = true;
public:
    void clean();

    void playMusic(const std::string& id);
    void loadMusic(const std::string& id, const std::string& source);

    void playEffect(const std::string& id);
    void loadEffect(const std::string& id, const std::string& source);
    bool parseSounds(const std::string& source);

    static SoundManager *getInstance();
    bool getMusicSetting() const{return music;}
    void update(Warrior *player);
};


#endif //ANIMALGAME_SOUNDMANAGER_H
