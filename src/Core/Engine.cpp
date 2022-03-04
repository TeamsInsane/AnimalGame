//
// Created by TeamsInsane on 15. 02. 2022.
//

#include "Engine.h"
#include <iostream>
#include "SDL.h"
#include <SDL_ttf.h>
#include "../Graphics/TextureManager.h"
#include <SDL_image.h>
#include "../Characters/Warrior.h"
#include "../Inputs/Input.h"
#include "../Timers/Timer.h"
#include "../Maps/MapParser.h"
#include "../Factory/ObjectFactory.h"
#include "../Sound/SoundManager.h"
#include "../ScreenText/TextDisplays.h"

Engine* Engine::instance = nullptr;
Warrior *player = nullptr;

Engine *Engine::getInstance() {
    if (instance == nullptr) instance = new Engine;
    return instance;
}

bool Engine::init(){
    if (TTF_Init() == -1){
        SDL_Log("Could not initialize SDL2 ttf, error: %s", TTF_GetError());
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_WindowFlags flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    window = SDL_CreateWindow("Minecraft 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    if(window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (!MapParser::getInstance()->load("../assets/maps/map.tmx")){
        SDL_Log("failed to load map: %s", SDL_GetError());
        return false;
    }

    if (!SoundManager::getInstance()->parseSounds("../assets/sounds.tml")){
        SDL_Log("failed to parse sounds: %s", SDL_GetError());
        return false;
    }

    levelMap = MapParser::getInstance()->getMaps("MAP");

    TileLayer *collisionLayer = (TileLayer*) levelMap->getMapLayers().back();

    parallaxBg.push_back(new ImgLayer("bg", 0, 0, 1920, 1080, 0.2, 1, 1));

    int tileSize = collisionLayer->getTileSize();
    int width = collisionLayer->getWidth()*tileSize;
    int height = collisionLayer->getHeight()*tileSize;

    Camera::getInstance()->setSceneLimit(width, height);

    CollisionHandler::getInstance()->setCollisionMap(collisionLayer->getTileMap(), tileSize);

    TextureManager::getInstance()->parseTextures("../assets/textures.tml");

    player = new Warrior(new Properties("player", 100, 200, 32, 32));

    //Properties *playerProperties = new Properties("player", 100, 200, 32, 32);
    //GameObject *player = ObjectFactory::getInstance()->createObject("PLAYER", playerProperties);

    Properties *animalProperties = new Properties("animal_idle", 150, 200, 48, 48);
    GameObject *animal = ObjectFactory::getInstance()->createObject("ANIMAL", animalProperties);

    Properties *bossProperties = new Properties("boss_idle", 400, 100, 250, 250);
    GameObject *boss = ObjectFactory::getInstance()->createObject("ENEMY", bossProperties);

    gameObject.push_back(player);
    gameObject.push_back(boss);
    gameObject.push_back(animal);
    Camera::getInstance()->setTarget(player->getOrigin());

    SoundManager::getInstance()->playMusic("banger");

    running = true;
    return running;
}
void Engine::update(){
    float dt = Timer::getInstance()->getDeltaTime();
    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->update(dt);
    player->update(dt);
    SoundManager::getInstance()->update();
    if (!SoundManager::getInstance()->getMusicSetting() && Mix_PlayingMusic()) Mix_PauseMusic();
    if (player->getIsJumpingOrFalling() && SoundManager::getInstance()->getMusicSetting()){
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }

    Camera::getInstance()->update(dt);

    levelMap->update();
}

void Engine::events() {
    Input::getInstance()->listen();
}

void Engine::render(){
    SDL_SetRenderDrawColor(renderer, 124, 210, 254, 255);
    SDL_RenderClear(renderer);

    for(int i = 0; i != parallaxBg.size(); i++) parallaxBg[i]->render();
    //TextureManager::getInstance()->drawTexture("bg", 0, 0, 1920, 1080, 0.5, 0.5, 0.5);
    levelMap->render();

    player->draw();
    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->draw();

    TextDisplays::getInstance()->draw(player->getOrigin()->x, player->getOrigin()->y);

    SDL_RenderPresent(renderer);
}

void Engine::clean(){
    player->clean();
    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->clean();
    TextureManager::getInstance()->cleanTexture();
    MapParser::getInstance()->clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Engine::quit(){
    running = false;
}