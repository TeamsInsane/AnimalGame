//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Play.h"

#include <utility>
#include <fstream>
#include "../Maps/MapParser.h"
#include "../Graphics/TextureManager.h"
#include "../Sound/SoundManager.h"
#include "../Timers/Timer.h"
#include "../Leaderboards/Leaderboard.h"
#include "../Replays/Replay.h"

Play *Play::instance = nullptr;

Play *Play::getInstance() {
    if (instance == nullptr)
        instance = new Play();
    return instance;
}

void Play::gameInit(const std::string& id, const std::string& src, SDL_Renderer *sdlRenderer){
    std::ofstream replayData("Files/ReplayFiles/Replay.txt");
    replayData << Engine::getInstance()->getLevel() << std::endl;
    replayData.close();
    remove("Files/ReplayFiles/Replay.bin");
    this->renderer = sdlRenderer;
    int y;

    if (Engine::getInstance()->getLevel() == 1) {
        startTime = SDL_GetTicks() / 1000;
        enemies.push_back(new Enemy(new Properties("ghost", 1800, 2080, 32, 32)));
        setSpawnLocations1();
        y = 2150;
        safeX1 = 1390;
        safeX2 = 1620;
        safeY = 2150;
        savedAnimals = 0;
    } else {
        setSpawnLocations2();
        y = 2045;
        safeX1 = 1100;
        safeX2 = 1365;
        safeY = 2000;
        enemies.push_back(new Enemy(new Properties("ghost", 830, 1730, 32, 32)));
        enemies.push_back(new Enemy(new Properties("ghost", 2290, 1730, 32, 32)));
    }

    player = new Warrior(new Properties("player", rand()%(2800-1040)+1040, y, 32, 32));

    player->setName(Menu::getInstance()->getName());

    if (!MapParser::getInstance()->load(id, src)){
        SDL_Log("Failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SoundManager::getInstance()->parseSounds("../assets/sounds.tml")){
        SDL_Log("Failed to parse sounds: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    initialized = false;
    displayGameOver = false;
    displayVictory = false;
    savedAnimalsPerLevel = 0;
    remainingAnimals = 7;
    currentTime = 0;
    index = -1;
    delay = 0;

    SoundManager::getInstance()->turnOnMusic();

    heartTexture = SDL_CreateTextureFromSurface(sdlRenderer, IMG_Load("../assets/menu/hearts.png"));

    levelMap = MapParser::getInstance()->getMaps(id);

    TileLayer *collisionLayer = (TileLayer*) levelMap->getMapLayers().back();

    parallaxBg.push_back(new ImgLayer("bg", 0, 0, 3840, 2160, 0.2, 1, 1));
    parallaxBg.push_back(new ImgLayer("cloud", 500, 800, 784, 253, 0.5, 0.7, 0.7));
    parallaxBg.push_back(new ImgLayer("cloud", 1700, 650, 784, 253, 0.5, 0.7, 0.7));

    int tileSize = collisionLayer->getTileSize();
    int width = collisionLayer->getWidth()*tileSize;
    int height = collisionLayer->getHeight()*tileSize;

    Camera::getInstance()->setSceneLimit(width, height);

    CollisionHandler::getInstance()->setCollisionMap(collisionLayer->getTileMap(), tileSize);

    TextureManager::getInstance()->parseTextures("../assets/textures.tml");

    animals.push_back(renderAnimal());

    SoundManager::getInstance()->playMusic("banger");
}

void Play::gameRender(){
    for (int i = 0; i != parallaxBg.size(); i++) parallaxBg[i]->render();
    levelMap->render();

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

    Text tempText = Text();
    std::string temp = "x: " + std::to_string(player->getOrigin()->x) + " y: " + std::to_string(player->getOrigin()->y);
    tempText.init(renderer, 0, 30, temp.c_str());
    temp = "Saved animals: " + std::to_string(savedAnimalsPerLevel) + ", remaining animals: " + std::to_string(remainingAnimals);
    tempText.init(renderer, 0, 0, temp.c_str());

    Timer::getInstance()->displayTime();
}

void Play::gameUpdate(){
    Replay::getInstance()->saveMovement();
    Menu::getInstance()->checkMenu(renderer);
    float dt = Timer::getInstance()->getDeltaTime();

    for (int i = 0; i != enemies.size(); i++) enemies[i]->update(dt);
    player->update(dt);
    for(int i = 0; i < animals.size(); i++) animals[i]->update(dt);

    SDL_Rect playerRect = player->getBox();

    for(int i = 0; i < animals.size() && index == -1; i++) {
        SDL_Rect animalRect = animals[i]->getBox();
        if (SDL_HasIntersection(&playerRect, &animalRect)) index = i;
    }

    if (index != -1){
        SDL_Rect animalRect = animals[index]->getBox();
        if (SDL_HasIntersection(&playerRect, &animalRect)) {
            animals[index]->setX(player->getOrigin()->x - 15);
            animals[index]->setY(player->getOrigin()->y - 30);
            if (animalRect.x > safeX1 && animalRect.x < safeX2 && animalRect.y > safeY) {
                savedAnimalsPerLevel++;
                savedAnimals++;
                remainingAnimals--;
                for(int i = 0; i < rand()%(5-2)+2; i++) enemies.push_back(renderEnemy());
                delete (animals[index]);
                animals.erase(animals.begin() + index);
                animals.push_back(Play::getInstance()->renderAnimal());
                index = -1;
            }
        }
    }

    delay++;
    if (delay > 100) player->resetIsHurt();
    for(int i = 0; i < enemies.size() && delay > 100; i++){
        SDL_Rect enemiesRect = enemies[i]->getBox();
        if (SDL_HasIntersection(&playerRect, &enemiesRect)){
            delay = 0;
            player->changeHealth(player->getHealth() - 1);
            SoundManager::getInstance()->playEffect("hurt");
            player->setIsHurt();
        }
    }

    if (player->getHealth() == 0) {
        gameClean();
        displayGameOver = true;
    }

    Camera::getInstance()->update();

    levelMap->update();

    if (remainingAnimals <= 6){
        if (Engine::getInstance()->getLevel() == 1) {
            MapParser::getInstance()->clean();
            gameClean();
            Engine::getInstance()->setLevel(2);
        } else if (Engine::getInstance()->getLevel() == 2){
            displayVictory = true;
            MapParser::getInstance()->clean();
            endScreen(true);
            Engine::getInstance()->setLevel(1);
        }
    }
}

void Play::gameClean(){
    SoundManager::getInstance()->turnOffMusic();
    if (player != nullptr) {
        Leaderboard::getInstance()->addToFile(player->getName(), savedAnimals);
        player->clean();
        delete player;
        player = nullptr;
    }
    for(int i = 0; i < animals.size(); i++) animals[i]->clean();
    animals.clear();
    for (int i = 0; i != enemies.size(); i++) enemies[i]->clean();
    enemies.clear();
    animalSpawnLocations.clear();
    enemySpawnLocations.clear();
    parallaxBg.clear();
    MapParser::getInstance()->clean();
    SDL_DestroyTexture(heartTexture);
}

void Play::endScreen(bool won){
    if (!initialized){
        Engine::getInstance()->setLevel(1);
        Menu::getInstance()->resetDisplayGame();
        char tempText[50] = "GAME OVER";
        if (won) strcpy(tempText, "You won");
        gameOverText.initCenter(renderer, SCREEN_HEIGHT / 2, 100, tempText);
        SDL_Surface *surface = IMG_Load("../assets/images/victory.png");
        if (won) victoryScreen = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), surface);
        surface = IMG_Load("../assets/images/menuBackground.png");
        background = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), surface);
        victoryRect = {SCREEN_WIDTH / 2 - 300, 100, 600, 200};
        bgRect = {0, 0, 1920, 1080};
        SDL_FreeSurface(surface);
        gameClean();
        initialized = true;
    }
    SDL_RenderCopyEx(renderer, background, nullptr, &bgRect, 0, nullptr, SDL_FLIP_NONE);
    if (won) SDL_RenderCopyEx(renderer, victoryScreen, nullptr, &victoryRect, 0, nullptr, SDL_FLIP_NONE);
    gameOverText.draw();
}

Animals *Play::renderAnimal(){
    auto it = animalSpawnLocations.begin();
    for(int i = 0; i < rand()%(animalSpawnLocations.size() - 0) + 0; i++) it++;
    float x = it->first, y = it->second;
    switch (rand()%(11-0)+0) {
        case 0:
            return new Animals(new Properties("bird", x, y, 32, 32));
        case 1:
            return new Animals(new Properties("cat1", x, y, 48, 48));
        case 2:
            return new Animals(new Properties("cat2", x, y, 48, 48));
        case 3:
            return new Animals(new Properties("chicken", x, y, 32, 32));
        case 4:
            return new Animals(new Properties("dog1", x, y, 48, 48));
        case 5:
            return new Animals(new Properties("dog2", x, y, 48, 48));
        case 6:
            return new Animals(new Properties("hyena", x, y, 48, 48));
        case 7:
            return new Animals(new Properties("rat", x, y, 32, 32));
        case 8:
            return  new Animals(new Properties("scorpion", x, y, 48, 48));
        case 9:
            return new Animals(new Properties("snake", x, y, 48, 48));
        case 10:
            return new Animals(new Properties("frog", x, y, 32, 32));
        default:
            exit(EXIT_FAILURE);
    }
}

Enemy *Play::renderEnemy(){
    if (enemySpawnLocations.empty()) setEnemiesSpawn();
    auto it = enemySpawnLocations.begin();
    for(int i = 1; i < rand()%(enemySpawnLocations.size() + 1 - 0) - 0; i++) it++;
    int x = it->first, y = it->second;
    enemySpawnLocations.erase(it);
    return new Enemy(new Properties("ghost", x - 100, y, 32, 32));
}

void Play::resetDisplayGameOver(){
    displayGameOver = false;
    displayVictory = false;
}

void Play::setSpawnLocations1(){
    animalSpawnLocations.insert(std::pair<int, int>(1910, 2060));
    animalSpawnLocations.insert(std::pair<int, int>(2230, 1937));
    animalSpawnLocations.insert(std::pair<int, int>(1470, 1850));
    animalSpawnLocations.insert(std::pair<int, int>(1150, 1680));
    animalSpawnLocations.insert(std::pair<int, int>(2100, 1750));
    animalSpawnLocations.insert(std::pair<int, int>(1440, 1490));
    animalSpawnLocations.insert(std::pair<int, int>(2240, 1520));
    animalSpawnLocations.insert(std::pair<int, int>(1910, 1350));
    animalSpawnLocations.insert(std::pair<int, int>(2595, 1680));

    setEnemiesSpawn();
}

void Play::setSpawnLocations2(){
    animalSpawnLocations.insert(std::pair<int, int>(930, 1730));
    animalSpawnLocations.insert(std::pair<int, int>(1025, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(1620, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(2265, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(2390, 1730));
    animalSpawnLocations.insert(std::pair<int, int>(930, 1730));
    animalSpawnLocations.insert(std::pair<int, int>(1025, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(1620, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(2265, 1400));
    animalSpawnLocations.insert(std::pair<int, int>(2390, 1730));

    setEnemiesSpawn();
}

void Play::setEnemiesSpawn(){
    for(auto it = animalSpawnLocations.begin(); it != animalSpawnLocations.end(); it++) enemySpawnLocations.insert(std::pair<int, int>(it->first, it->second));
}

void Play::loadInformationData(int savedPerLevel, int saved, int remaining, int prev){
    savedAnimalsPerLevel = savedPerLevel;
    remainingAnimals = remaining;
    savedAnimals = saved;
    currentTime = prev;
}

void Play::loadPlayerData(int health, float x, float y){
    player->changeHealth(health);
    player->setX(x);
    player->setY(y);
}

void Play::loadAnimalData(float x, float y){
    SDL_Log("Loaded animal!");
    animals.push_back(renderAnimal());
    animals[animals.size() - 1]->setX(x);
    animals[animals.size() - 1]->setY(y);
}

void Play::loadEnemyData(float x, float y, int moveCount, bool moveLeft){
    enemies.push_back(renderEnemy());
    enemies[enemies.size() - 1]->setX(x);
    enemies[enemies.size() - 1]->setY(y);
    enemies[enemies.size() - 1]->setMove(moveCount, moveLeft);
}

void Play::loadClean() {
    animals.clear();
    enemies.clear();
}

std::vector<Enemy*> Play::getEnemies(){return enemies;}
std::vector<Animals*> Play::getAnimals(){return animals;}
int Play::getRemainingAnimals() const{return remainingAnimals;}
int Play::getSavedAnimalsPerLevel() const{ return savedAnimalsPerLevel;}
int Play::getHealth() const{return player->getHealth();}
bool Play::getDisplayVictory() const{return displayVictory;}
int Play::getCurrentTime() const {return currentTime;}
int Play::getStartTime() const{return startTime;}
std::string Play::getPlayerNameForMenu() {return player->getName();}
float Play::getPlayerPositionX() {return player->getOrigin()->x;}
float Play::getPlayerPositionY() {return player->getOrigin()->y;}
bool Play::getDisplayGameOver() const {return displayGameOver;}
int Play::getSavedAnimals() const {return savedAnimals;}
