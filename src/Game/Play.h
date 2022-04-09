//
// Created by TeamsInsane on 4. 03. 2022.
//

#ifndef ANIMALGAME_PLAY_H
#define ANIMALGAME_PLAY_H


#include <SDL.h>
#include <map>
#include "../Maps/GameMap.h"
#include "../Core/Engine.h"
#include "../Characters/Warrior.h"
#include "../Characters/Animals.h"
#include "../Characters/Enemy.h"

class Play {
private:
    static Play* instance;
    GameMap *levelMap;
    std::vector<ImgLayer*> parallaxBg;
    Warrior *player;
    std::vector<Enemy*> enemies;
    std::vector<Animals*> animals;
    int savedAnimals;
    int remainingAnimals;
    int index;
    int delay;
    bool initialized;
    int enemySize;
    int startTime;
    SDL_Texture *heartTexture;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Rect bgRect;
    Text gameOverText;
    std::multimap<int, int> animalSpawnLocations, enemySpawnLocations;
    bool displayGameOver;
public:
    static Play *getInstance();

    Animals *renderAnimal();

    void gameUpdate();

    void gameRender();

    void gameInit(std::string id, std::string src, SDL_Renderer *renderer);

    void gameClean();

    std::string getPlayerNameForMenu();

    void gameOver();

    float getPlayerPositionX();

    float getPlayerPositionY();

    bool getDisplayGameOver() const;

    void resetDisplayGameOver();

    void setSpawnLocations();

    Enemy * renderEnemy();

    void setEnemiesSpawn();

    int getStartTime();
};


#endif //ANIMALGAME_PLAY_H
