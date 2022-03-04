//
// Created by TeamsInsane on 15. 02. 2022.
//

#include "Engine.h"
#include <iostream>
#include "SDL.h"
#include <SDL_ttf.h>
#include "../Graphics/TextureManager.h"
#include <SDL_image.h>
#include "../Inputs/Input.h"
#include "../Timers/Timer.h"
#include "../Maps/MapParser.h"
#include "../Sound/SoundManager.h"
#include "../ScreenText/TextDisplays.h"
#include "../Game/Play.h"

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

    Play::getInstance()->mainGame(levelMap, parallaxBg, player, gameObject);

    running = true;
    return running;
}
void Engine::update(){
    float dt = Timer::getInstance()->getDeltaTime();
    for(int i = 0; i != gameObject.size(); i++) gameObject[i]->update(dt);
    player->update(dt);
    SoundManager::getInstance()->update(player);

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
