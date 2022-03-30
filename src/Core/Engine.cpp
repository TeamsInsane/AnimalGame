//
// Created by TeamsInsane on 15. 02. 2022.
//

#include "Engine.h"
#include "../Characters/Warrior.h"
#include "../Characters/Animals.h"
#include "../Timers/Timer.h"
#include "../Sound/SoundManager.h"
#include "../Inputs/Input.h"
#include "../Game/Play.h"
#include "../Graphics/TextureManager.h"
#include "../Maps/MapParser.h"

Engine* Engine::instance = nullptr;

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

    Menu::getInstance()->init(renderer);

    working = false;
    running = true;
    return running;
}
void Engine::update(){
    if (!Menu::getInstance()->getDisplayGame() && !Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->update();
    else if (Menu::getInstance()->getDisplayGame()) Play::getInstance()->gameUpdate();
    else if (Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->checkMenu(renderer);
}

void Engine::events() {
    Input::getInstance()->listen();
}

void Engine::render(){
    SDL_RenderClear(renderer);
    if (!Menu::getInstance()->getDisplayGame() && !Menu::getInstance()->getDisplayDirections())Menu::getInstance()->draw(renderer);
    else if (Menu::getInstance()->getDisplayGame()){
        if (!working){
            Play::getInstance()->gameInit();
            working = true;
        }
        Play::getInstance()->gameRender();
    }  else if (Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->loadDirections(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::clean(){
        Play::getInstance()->gameClean();
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
