//
// Created by TeamsInsane on 15. 02. 2022.
//

#include "Engine.h"
#include <iostream>
#include "SDL.h"
#include "../Graphics/TextureManager.h"
#include <SDL_image.h>
#include "../Characters/Warrior.h"
#include "../Inputs/Input.h"
#include "../Timers/Timer.h"
#include "../Maps/MapParser.h"
#include "../Cameras/Camera.h"
#include "../Characters/Enemy.h"
#include "../Factory/ObjectFactory.h"

Engine* Engine::instance = nullptr;

Engine *Engine::getInstance() {
    if (instance == nullptr) instance = new Engine;
    return instance;
}

bool Engine::init(){
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

    levelMap = MapParser::getInstance()->getMaps("MAP");
    TextureManager::getInstance()->parseTextures("../assets/textures.tml");

    Properties *playerProperties = new Properties("player", 100, 200, 32, 32);
    GameObject *player = ObjectFactory::getInstance()->createObject("PLAYER", playerProperties);

    Properties *bossProperties = new Properties("boss_idle", 400, 100, 250, 250);
    GameObject *boss = ObjectFactory::getInstance()->createObject("ENEMY", bossProperties);

    gameObject.push_back(player);
    gameObject.push_back(boss);
    Camera::getInstance()->setTarget(player->getOrigin());

    running = true;
    return running;
}
void Engine::update(){
    float dt = Timer::getInstance()->getDeltaTime();

    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->update(dt);

    Camera::getInstance()->update(dt);

    levelMap->update();
}

void Engine::events() {
    Input::getInstance()->listen();
}

void Engine::render(){
    SDL_SetRenderDrawColor(renderer, 124, 210, 254, 255);
    SDL_RenderClear(renderer);

    TextureManager::getInstance()->drawTexture("bg", 0, 0, 1920, 1080, 0.5, 0.5, 0.5);
    levelMap->render();

    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->draw();

    SDL_RenderPresent(renderer);
}

void Engine::clean(){
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