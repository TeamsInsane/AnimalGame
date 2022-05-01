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
    int startTime, currentTime;
    int safeX1, safeX2, safeY;
    SDL_Texture *heartTexture;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Texture *victoryScreen;
    SDL_Rect bgRect, victoryRect;
    Text gameOverText;
    std::multimap<int, int> animalSpawnLocations, enemySpawnLocations;
    bool displayGameOver;
    bool displayVictory;
public:
    static Play *getInstance();

    Animals *renderAnimal();
    Enemy *renderEnemy();

    void gameUpdate();
    void gameRender();
    void gameInit(const std::string& id, std::string src, SDL_Renderer *sdlRenderer);
    void gameClean();


    void resetDisplayGameOver();
    void setSpawnLocations1();
    void setEnemiesSpawn();
    void setSpawnLocations2();
    void endScreen(bool won);

    int getStartTime() const;
    bool getDisplayVictory() const;
    std::vector<Enemy *> getEnemies();
    int getSavedAnimals() const;
    int getRemainingAnimals() const;
    std::vector<Animals *> getAnimals();
    int getHealth() const;
    int getCurrentTime() const;
    float getPlayerPositionX();
    float getPlayerPositionY();
    bool getDisplayGameOver() const;
    std::string getPlayerNameForMenu();

    void loadInformationData(int saved, int remaining, int prev);
    void loadPlayerData(int health, float x, float y);
    void loadAnimalData(float x, float y);
    void loadClean();
    void loadEnemyData(float x, float y, int moveCount, bool moveLeft);
};


#endif //ANIMALGAME_PLAY_H
