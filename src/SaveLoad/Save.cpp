//
// Created by TeamsInsane on 30. 04. 2022.
//

#include "Save.h"
#include "../Game/Play.h"
#include "../Timers/Timer.h"
#include <fstream>
#include <sys/stat.h>

Save *Save::instance = nullptr;

Save *Save::getInstance() {
    if (instance == nullptr) instance = new Save();
    return instance;
}

void Save::saveGameplay() {
    if (!Engine::getInstance()->getInitialized()){
        SDL_Log("Nothing to save!");
        return;
    }
    struct SaveInformation saveInformation = getInformationData();
    std::ofstream data("Files/SaveFiles/Save.bin", std::ios::binary);
    data.write((char *) &saveInformation, sizeof saveInformation);
    data.close();

    struct PlayerInformation playerInformation = getPlayerData();
    std::ofstream playerData("Files/SaveFiles/PlayerSave.bin", std::ios::binary);
    playerData.write((char *) &playerInformation, sizeof playerInformation);
    playerData.close();

    std::vector<AnimalInformation> animalInformation = getAnimalData();
    std::ofstream animalSave("Files/SaveFiles/AnimalSave.bin", std::ios::binary);
    for(int i = 0; i < animalInformation.size(); i++)
        animalSave.write((char *) &animalInformation[i], sizeof animalInformation[i]);

    animalSave.close();

    std::vector<EnemyInformation> enemyInformation = getEnemyData();
    std::ofstream enemyData("Files/SaveFiles/EnemySave.bin", std::ios::binary);
    for(int i = 0; i < enemyInformation.size(); i++)
        enemyData.write((char *) &enemyInformation[i], sizeof enemyInformation[i]);

    enemyData.close();
}

void Save::loadGameplay() {
    struct SaveInformation saveInformation = SaveInformation();
    std::ifstream data("Files/SaveFiles/Save.bin", std::ios::binary);
    if (!data.is_open()){
        SDL_Log("Couldn't open file!");
        return;
    }

    data.read((char *) &saveInformation, sizeof saveInformation);
    data.close();

    Engine::getInstance()->setLevel(saveInformation.level);
    if (saveInformation.level == 1)
        Play::getInstance()->gameInit("Level1", "../assets/maps/level1.tmx", Engine::getInstance()->getRenderer());
    else
        Play::getInstance()->gameInit("Level2", "../assets/maps/level2.tmx", Engine::getInstance()->getRenderer());
    Engine::getInstance()->changeInitialized(true);
    Play::getInstance()->loadInformationData(saveInformation.savedAnimalsPerLevel, saveInformation.savedAnimals, saveInformation.remainingAnimals, saveInformation.prevTime);

    Play::getInstance()->loadClean();

    struct PlayerInformation playerInformation = PlayerInformation();
    std::ifstream playerData("Files/SaveFiles/PlayerSave.bin", std::ios::binary);
    playerData.read((char *) &playerInformation, sizeof playerInformation);
    playerData.close();

    Play::getInstance()->loadPlayerData(playerInformation.health, playerInformation.x - 16, playerInformation.y - 45);

    std::vector<AnimalInformation> animalInformation;
    std::ifstream animalSave("Files/SaveFiles/AnimalSave.bin", std::ios::binary);
    struct AnimalInformation animalTemp = AnimalInformation();
    while (animalSave.read((char *) &animalTemp, sizeof animalTemp))
        animalInformation.push_back(animalTemp);
    animalSave.close();

    for(int i = 0; i < animalInformation.size(); i++)
        Play::getInstance()->loadAnimalData(animalInformation[i].x - 16, animalInformation[i].y - 45);

    std::vector<EnemyInformation> enemyInformation;
    std::ifstream enemyData("Files/SaveFiles/EnemySave.bin", std::ios::binary);
    struct EnemyInformation enemyTemp = EnemyInformation();
    while (enemyData.read((char *) &enemyTemp, sizeof enemyTemp))
        enemyInformation.push_back(enemyTemp);
    enemyData.close();

    for(int i = 0; i < enemyInformation.size(); i++)
        Play::getInstance()->loadEnemyData(enemyInformation[i].x - 16, enemyInformation[i].y - 20, enemyInformation[i].moveCount, enemyInformation[i].moveLeft);

}

SaveInformation Save::getInformationData(){
    struct SaveInformation temp = SaveInformation();
    temp.level = Engine::getInstance()->getLevel();
    temp.savedAnimalsPerLevel = Play::getInstance()->getSavedAnimalsPerLevel();
    temp.savedAnimals = Play::getInstance()->getSavedAnimals();
    temp.remainingAnimals = Play::getInstance()->getRemainingAnimals();
    temp.prevTime = Timer::getInstance()->getCurrentTime();

    return temp;
}

PlayerInformation Save::getPlayerData(){
    struct PlayerInformation temp = PlayerInformation();
    temp.health = Play::getInstance()->getHealth();
    temp.x = Play::getInstance()->getPlayerPositionX();
    temp.y = Play::getInstance()->getPlayerPositionY();

    return temp;
}

std::vector<AnimalInformation> Save::getAnimalData(){
    std::vector<Animals*> animals = Play::getInstance()->getAnimals();
    std::vector<AnimalInformation> temp(animals.size());
    for(int i = 0; i < animals.size(); i++){
        temp[i].x = animals[i]->getOrigin()->x;
        temp[i].y = animals[i]->getOrigin()->y;
    }

    return temp;
}

std::vector<EnemyInformation> Save::getEnemyData(){
    std::vector<Enemy*> enemies = Play::getInstance()->getEnemies();
    std::vector<EnemyInformation> temp(enemies.size());
    for(int i = 0; i < enemies.size(); i++){
        temp[i].x = enemies[i]->getOrigin()->x;
        temp[i].y = enemies[i]->getOrigin()->y;
        temp[i].moveLeft = enemies[i]->getMoveLeft();
        temp[i].moveCount = enemies[i]->getMoveCount();
    }

    return temp;
}