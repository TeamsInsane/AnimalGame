//
// Created by TeamsInsane on 4. 03. 2022.
//

#ifndef ANIMALGAME_PLAY_H
#define ANIMALGAME_PLAY_H


#include <SDL.h>
#include "../Maps/GameMap.h"
#include "../Core/Engine.h"
#include "../Characters/Warrior.h"
#include "../Characters/Animals.h"
#include "../Characters/Enemy.h"

class Play {
private:
    int x = 1910;
    static Play* instance;
    GameMap *levelMap;
    std::vector<ImgLayer*> parallaxBg;
    Warrior *player;
    std::vector<Enemy*> enemies;
    std::vector<Animals*> animals;
    int savedAnimals;
    int index;
    int delay;
    SDL_Texture *heartTexture;
public:
    static Play *getInstance();

    Animals *renderAnimal();

    void gameUpdate();

    void gameRender();

    void gameInit(std::string id, std::string src);

    void gameClean();

    std::string getPlayerNameForMenu();
};


#endif //ANIMALGAME_PLAY_H
