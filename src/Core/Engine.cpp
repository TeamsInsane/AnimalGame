//
// Created by TeamsInsane on 15. 02. 2022.
//

#include <fstream>
#include "Engine.h"
#include "../Characters/Warrior.h"
#include "../Characters/Animals.h"
#include "../Sound/SoundManager.h"
#include "../Inputs/Input.h"
#include "../Game/Play.h"
#include "../Graphics/TextureManager.h"
#include "../Maps/MapParser.h"
#include "../Leaderboards/Leaderboard.h"
#include "../Replays/Replay.h"

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

    auto flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

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
    Replay::getInstance()->initReplay(renderer);

    level = 1;
    delay = 0;
    initialized = false;
    running = true;
    return running;
}

void Engine::update(){
    delay++;
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_F11) && delay > 100){
        if (!fullscreen){
            SDL_Log("Changed screen to fullscreen");
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            fullscreen = true;
        } else {
            SDL_Log("Changed screen to window");
            SDL_SetWindowFullscreen(window, 0);
            fullscreen = false;
        }
        delay = 0;
    }
    if (Menu::getInstance()->getDisplayMenu()) Menu::getInstance()->update();
    else if (Play::getInstance()->getDisplayGameOver() || Play::getInstance()->getDisplayVictory()) Menu::getInstance()->resetMenu(renderer);
    else if (Menu::getInstance()->getDisplayGame()) Play::getInstance()->gameUpdate();
    else if (Menu::getInstance()->getDisplayDirections() || Menu::getInstance()->getDisplayLeaderboard()) Menu::getInstance()->checkMenu(renderer);
    else if (Menu::getInstance()->getDisplayReplay()) Replay::getInstance()->update();
}

void Engine::events() {
    Input::getInstance()->listen();
}

void Engine::render(){
    SDL_RenderClear(renderer);
    if (Menu::getInstance()->getDisplayMenu()) Menu::getInstance()->draw(renderer);
    else if (Play::getInstance()->getDisplayGameOver()) Play::getInstance()->endScreen(false);
    else if (Play::getInstance()->getDisplayVictory()) Play::getInstance()->endScreen(true);
    else if (Menu::getInstance()->getDisplayGame()){
        switch(level) {
            case 1:
                if (!initialized) {
                    SDL_Log("Test");
                    std::ofstream data("Replay.txt");
                    data.close();
                    Play::getInstance()->gameInit("Level1", "../assets/maps/level1.tmx", renderer);
                    initialized = true;
                }
                break;
            case 2:
                if (!initialized) {
                    Play::getInstance()->gameInit("Level2", "../assets/maps/level2.tmx", renderer);
                    initialized = true;
                }
                break;
            default:
                exit(EXIT_FAILURE);
        }
        Play::getInstance()->gameRender();
    } else if (Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->loadDirections(renderer);
    else if (Menu::getInstance()->getDisplayLeaderboard()) Leaderboard::getInstance()->draw();
    else if (Menu::getInstance()->getDisplayReplay()) Replay::getInstance()->render();
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

void Engine::setLevel(int num){
    initialized = false;
    level = num;
}