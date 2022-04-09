//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Play.h"
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

void Play::gameInit(std::string id, std::string src, SDL_Renderer *renderer){
    if (id == "Level1") startTime = SDL_GetTicks()/1000;
    SDL_Log("Crash");
    setSpawnLocations();
    SDL_Log("animal %i, enemies %i", enemySpawnLocations.size(), animalSpawnLocations.size());
    this->renderer = renderer;
    player = new Warrior(new Properties("player", rand()%(2800-1040)+1040, 2150, 32, 32));

    player->setName("Guest1");

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
    savedAnimals = 0;
    remainingAnimals = 7;
    index = -1;
    delay = 0;
    enemySize = 0;

    heartTexture = SDL_CreateTextureFromSurface(renderer, IMG_Load("../assets/menu/hearts.png"));

    levelMap = MapParser::getInstance()->getMaps(id);

    TileLayer *collisionLayer = (TileLayer*) levelMap->getMapLayers().back();

    parallaxBg.push_back(new ImgLayer("bg", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.2, 2, 2));

    int tileSize = collisionLayer->getTileSize();
    int width = collisionLayer->getWidth()*tileSize;
    int height = collisionLayer->getHeight()*tileSize;

    Camera::getInstance()->setSceneLimit(width, height);

    CollisionHandler::getInstance()->setCollisionMap(collisionLayer->getTileMap(), tileSize);

    TextureManager::getInstance()->parseTextures("../assets/textures.tml");

    for(int i = 0; i < 1; i++) animals.push_back(renderAnimal());

    enemies.push_back(new Enemy(new Properties("ghost", 1800, 2080, 32, 32)));

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

    Text tempText;
    std::string temp = "x: " + std::to_string(player->getOrigin()->x) + " y: " + std::to_string(player->getOrigin()->y);
    tempText.init(renderer, 0, 30, temp.c_str());
    temp = "Saved animals: " + std::to_string(savedAnimals) + ", remaining animals: " + std::to_string(remainingAnimals);
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
    SoundManager::getInstance()->update(player);

    SDL_Rect playerRect = player->getBox();

    for(int i = 0; i < animals.size() && index == -1; i++) {
        SDL_Rect animalRect = animals[i]->getBox();
        if (SDL_HasIntersection(&playerRect, &animalRect)) index = i;
    }

    if (enemies.size() - enemySize >= 5){
        setEnemiesSpawn();
        enemySize = enemies.size();
    }

    if (index != -1){
        SDL_Rect animalRect = animals[index]->getBox();
        if (SDL_HasIntersection(&playerRect, &animalRect)) {
            animals[index]->setX(player->getOrigin()->x - 15);
            animals[index]->setY(player->getOrigin()->y - 30);
            if (animalRect.x > 1390 && animalRect.x < 1620 && animalRect.y > 2150) {
                savedAnimals++;
                remainingAnimals--;
                for(int i = 0; i < rand()%(5-2)+2; i++) enemies.push_back(renderEnemy());
                animals.erase(animals.begin() + index);
                animals.push_back(Play::getInstance()->renderAnimal());
                index = -1;
            }
        }
    }

    delay++;
    if (delay > 100) player->resetIsHurt();
    for(int i = 0; i < enemies.size(); i++){
        SDL_Rect enemiesRect = enemies[i]->getBox();
        if (SDL_HasIntersection(&playerRect, &enemiesRect) && delay > 100){
            delay = 0;
            player->changeHealth(player->getHealth() - 1);
            SoundManager::getInstance()->playEffect("hurt");
            player->setIsHurt();
        }
    }

    if (player->getHealth() == 0) {
        displayGameOver = true;
    }

    Camera::getInstance()->update(dt);

    levelMap->update();

    if (remainingAnimals <= 0){
        MapParser::getInstance()->clean();
        gameClean();
        Engine::getInstance()->setLevel(2);
    }
}

void Play::gameClean(){
    if (player != nullptr) {
        Leaderboard::getInstance()->addToFile(player->getName(), savedAnimals);
        player->clean();
        player = nullptr;
    }
    for(int i = 0; i < animals.size(); i++) animals[i]->clean();
    animals.clear();
    for (int i = 0; i != enemies.size(); i++) enemies[i]->clean();
    enemies.clear();
    parallaxBg.clear();
    MapParser::getInstance()->clean();
    SDL_DestroyTexture(heartTexture);
}

void Play::gameOver(){
    if (!initialized) {
        Engine::getInstance()->setLevel(1);
        Menu::getInstance()->resetDisplayGame();
        char tempText[] = "GAME OVER!";
        gameOverText.initCenter(renderer, SCREEN_HEIGHT / 2, 100, tempText);
        SDL_Surface *surface = IMG_Load("../assets/images/menuBackground.png");
        background = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), surface);
        bgRect = {0, 0, 1920, 1080};
        SDL_FreeSurface(surface);
        gameClean();
        initialized = true;
    }
    SDL_RenderCopyEx(renderer, background, nullptr, &bgRect, 0, nullptr, SDL_FLIP_NONE);
    gameOverText.draw();
}

Animals *Play::renderAnimal(){
    auto it = animalSpawnLocations.begin();
    for(int i = 0; i < rand()%(animalSpawnLocations.size() - 0) + 0; i++) it++;
    int x = it->first, y = it->second;
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
    auto it = enemySpawnLocations.begin();
    for(int i = 0; i < rand()%(enemySpawnLocations.size() + 1) + 0; i++) it++;
    int x = it->first, y = it->second;
    enemySpawnLocations.erase(it);
    return new Enemy(new Properties("ghost", x - 100, y, 32, 32));
}

std::string Play::getPlayerNameForMenu() {return player->getName();}

float Play::getPlayerPositionX() {return player->getOrigin()->x;}

float Play::getPlayerPositionY() {return player->getOrigin()->y;}

bool Play::getDisplayGameOver() const {return displayGameOver;}

void Play::resetDisplayGameOver(){
    displayGameOver = false;
}

int Play::getStartTime(){return startTime;}

void Play::setSpawnLocations(){
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

void Play::setEnemiesSpawn(){
    for(auto it = animalSpawnLocations.begin(); it != animalSpawnLocations.end(); it++) enemySpawnLocations.insert(std::pair<int, int>(it->first, it->second));
}