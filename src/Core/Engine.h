//
// Created by TeamsInsane on 15. 02. 2022.
//

#ifndef ANIMALGAME_ENGINE_H
#define ANIMALGAME_ENGINE_H

#include <SDL.h>
#include <SDL_image.h>
#include "../Maps/GameMap.h"
#include "../Objects/GameObject.h"
#include "../Maps/ImgLayer.h"
#include "../Game/Menu.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1030

class Engine {
private:
    bool running;
    GameMap* levelMap;
    SDL_Window* window;
    SDL_Renderer* renderer;
    static Engine* instance;
    std::vector<ImgLayer*> parallaxBg;
    std::vector<GameObject*> gameObject;
    int savedAnimals;
public:
    static Engine* getInstance();
    GameMap *getGameMap() {return levelMap;}

    bool init();
    void clean();
    void quit();
    void update();
    void render();
    void events();

    int getSavedAnimalsCount() {return savedAnimals;}
    inline bool isRunning() const{return running;}
    inline SDL_Renderer *getRenderer() const{return renderer;}
    inline SDL_Window *getWindow() const{return window;}
};


#endif //ANIMALGAME_ENGINE_H
