//
// Created by Uporabnik on 27. 02. 2022.
//

#include "SoundManager.h"
#include "../Inputs/Input.h"
#include "../Characters/Warrior.h"
#include "../Core/Engine.h"

SoundManager *SoundManager::instance = nullptr;

SoundManager::SoundManager() {
    if (Mix_OpenAudio(FREQ, MIX_DEFAULT_FORMAT, 2, CHUNK_SIZE) < 0) SDL_Log("Error: %s", Mix_GetError());

}

SoundManager *SoundManager::getInstance() {
    if (instance == nullptr) instance = new SoundManager();
    return instance;
}

void SoundManager::playMusic(std::string id) {
    if(Mix_PlayMusic(musicMap[id], -1) == -1) SDL_Log("Error: %s : %s", Mix_GetError(), id.c_str());
    Mix_VolumeMusic(3);
}

void SoundManager::loadMusic(std::string id, std::string source) {
    Mix_Music *music = Mix_LoadMUS(source.c_str());
    if (music != nullptr) musicMap[id] = music;
    else SDL_Log("Error: %s : %s", Mix_GetError(), source.c_str());
}

void SoundManager::playEffect(std::string id) {
    if (Mix_PlayChannel(-1, effectMap[id], 0) == -1) SDL_Log("Error: %s : %s", Mix_GetError(), id.c_str());
}

void SoundManager::loadEffect(std::string id, std::string source) {
    Mix_Chunk *effect = Mix_LoadWAV(source.c_str());
    if (effect != nullptr) effectMap[id] = effect;
    else SDL_Log("Error: %s : %s", Mix_GetError(), source.c_str());
}

void SoundManager::clean() {
    for(MusicMap::iterator it = musicMap.begin(); it != musicMap.end(); it++) Mix_FreeMusic(it->second);
    for(EffectMap::iterator it = effectMap.begin(); it != effectMap.end(); it++) Mix_FreeChunk(it->second);

    musicMap.clear();
    effectMap.clear();
    SDL_Log("Sound system cleaned!");
}

bool SoundManager::parseSounds(std::string source) {
    TiXmlDocument xml;
    xml.LoadFile(source);

    if (xml.Error()){
        SDL_Log("Failed to load: %s", source.c_str());
        return false;
    }

    TiXmlElement *root = xml.RootElement();
    for(TiXmlElement *element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()){
        if (element->Value() == std::string("effect")){
            SoundManager::getInstance()->loadEffect(element->Attribute("id"), element->Attribute("source"));
            continue;
        }

        if (element->Value() == std::string("music")){
            SoundManager::getInstance()->loadMusic(element->Attribute("id"), element->Attribute("source"));
            continue;
        }
    }

    SDL_Log("Successfully parsed: %s", source.c_str());
    return true;
}

void SoundManager::update(Warrior *player) {
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_M)){
        music = false;
        SDL_Log("Music was turned off!");
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_N)){
        music = true;
        SDL_Log("Music was turned on!");
    }

    if (!getMusicSetting() && Mix_PlayingMusic()) Mix_PauseMusic();

    if (player->getIsJumpingOrFalling() && getMusicSetting()) Mix_ResumeMusic();
    else Mix_PauseMusic();
}

