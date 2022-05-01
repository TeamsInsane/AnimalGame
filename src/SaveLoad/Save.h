//
// Created by Uporabnik on 30. 04. 2022.
//

#ifndef ANIMALGAME_SAVE_H
#define ANIMALGAME_SAVE_H


#include <vector>

struct SaveInformation{
    int level;
    int remainingAnimals;
    int savedAnimals;
    int prevTime;
};

struct PlayerInformation{
    int health;
    float x, y;
};

struct AnimalInformation{
    float x, y;
};

struct EnemyInformation{
    float x, y;
    int moveCount;
    bool moveLeft;
};

class Save {
private:
    static Save *instance;
public:
    static Save *getInstance();

    void saveGameplay();
    void loadGameplay();

    SaveInformation getInformationData();
    PlayerInformation getPlayerData();
    std::vector<AnimalInformation> getAnimalData();
    std::vector<EnemyInformation> getEnemyData();
};


#endif //ANIMALGAME_SAVE_H
