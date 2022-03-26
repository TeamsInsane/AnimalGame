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
#include "../Characters/Enemy.h"

Engine* Engine::instance = nullptr;

std::vector<Animals*> animals;
std::vector<Enemy*> enemies;
Warrior *player;

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

    heartTexture = SDL_CreateTextureFromSurface(renderer, IMG_Load("../assets/menu/hearts.png"));

    savedAnimals = 0;
    index = -1;
    delay = 0;
    running = true;
    return running;
}
void Engine::update(){
    if (!Menu::getInstance()->getDisplayGame() && !Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->update();
    else if (Menu::getInstance()->getDisplayGame()){
        Menu::getInstance()->checkMenu(renderer);
        float dt = Timer::getInstance()->getDeltaTime();
        for (int i = 0; i != enemies.size(); i++) enemies[i]->update(dt);
        player->update(dt);\
        for(int i = 0; i < animals.size(); i++) animals[i]->update(dt);
        SoundManager::getInstance()->update(player);

        Camera::getInstance()->update(dt);

        levelMap->update();
    } else if (Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->checkMenu(renderer);
}

void Engine::events() {
    Input::getInstance()->listen();
}

void Engine::render(){
    SDL_RenderClear(renderer);
    if (!Menu::getInstance()->getDisplayGame() && !Menu::getInstance()->getDisplayDirections())Menu::getInstance()->draw(renderer);
    else if (Menu::getInstance()->getDisplayGame()){
        if (player == nullptr) Play::getInstance()->mainGame(levelMap, parallaxBg, player, enemies, animals);
        for (int i = 0; i != parallaxBg.size(); i++) parallaxBg[i]->render();
        levelMap->render();
        SDL_Rect playerRect = player->getBox();
        for(int i = 0; i < animals.size() && index == -1; i++) {
            SDL_Rect animalRect = animals[i]->getBox();
            if (SDL_HasIntersection(&playerRect, &animalRect)) index = i;
        }

        if (index != -1){
            SDL_Rect animalRect = animals[index]->getBox();
            if (SDL_HasIntersection(&playerRect, &animalRect)) {
                animals[index]->setX(player->getOrigin()->x);
                animals[index]->setY(player->getOrigin()->y - 30);
                if (animalRect.x > 604 && animalRect.x < 800 && animalRect.y > 2200) {
                    savedAnimals++;
                    animals.erase(animals.begin() + index);
                    animals.push_back(Play::getInstance()->renderAnimal());
                    SDL_Log("Animal erased and spawned a new one!");
                    index = -1;
                }
            }
        }

        for(int i = 0; i < enemies.size(); i++){
            SDL_Rect enemiesRect = enemies[i]->getBox();
            delay++;
            if (SDL_HasIntersection(&playerRect, &enemiesRect) && delay > 100){
                delay = 0;
                player->changeHealth(player->getHealth() - 1);
            }
        }
        int y = 25;
        for(int i = 0; i < player->getHealth(); i++){
            SDL_Rect heartRect;
            heartRect = {SCREEN_WIDTH - y, 0, 25, 25};
            y+=25;
            SDL_RenderCopyEx(renderer, heartTexture, nullptr, &heartRect, 0, nullptr, SDL_FLIP_NONE);
        }

        player->draw();
        for(int i = 0; i < animals.size(); i++) animals[i]->draw();
        for (int i = 0; i != enemies.size(); i++) enemies[i]->draw();

        Text tempText;
        std::string temp = "x: " + std::to_string(player->getOrigin()->x) + " y: " + std::to_string(player->getOrigin()->y);
        tempText.init(renderer, 0, 30, temp.c_str());
        temp = "Saved animals count: " + std::to_string(savedAnimals);
        tempText.init(renderer, 0, 0, temp.c_str());

    }  else if (Menu::getInstance()->getDisplayDirections()) Menu::getInstance()->loadDirections(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::clean(){
        player->clean();
        for(int i = 0; i < animals.size(); i++) animals[i]->clean();
        for (int i = 0; i != enemies.size(); i++) enemies[i]->clean();
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
