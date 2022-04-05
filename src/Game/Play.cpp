//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Play.h"
#include "../Maps/MapParser.h"
#include "../Graphics/TextureManager.h"
#include "../Sound/SoundManager.h"
#include "../Timers/Timer.h"
#include "../Leaderboards/Leaderboard.h"

Play *Play::instance = nullptr;

Play *Play::getInstance() {
    if (instance == nullptr) instance = new Play();
    return instance;
}

void Play::gameInit(std::string id, std::string src){
    player = new Warrior(new Properties("player", 1500, 1800, 32, 32));

    SDL_Log("Enter your name: ");
    std::string temp = "Teams";
    //fflush(stdin);
    //std::getline(std::cin, temp);
    player->setName(temp);

    if (!MapParser::getInstance()->load(id, src)){
        SDL_Log("Failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SoundManager::getInstance()->parseSounds("../assets/sounds.tml")){
        SDL_Log("Failed to parse sounds: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    savedAnimals = 0;
    index = -1;
    delay = 0;

    heartTexture = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), IMG_Load("../assets/menu/hearts.png"));

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
    Camera::getInstance()->setTarget(player->getOrigin());

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
        SDL_RenderCopyEx(Engine::getInstance()->getRenderer(), heartTexture, nullptr, &heartRect, 0, nullptr, SDL_FLIP_NONE);
    }

    player->draw();
    for(int i = 0; i < animals.size(); i++) animals[i]->draw();
    for (int i = 0; i != enemies.size(); i++) enemies[i]->draw();

    Text tempText;
    std::string temp = "x: " + std::to_string(player->getOrigin()->x) + " y: " + std::to_string(player->getOrigin()->y);
    tempText.init(Engine::getInstance()->getRenderer(), 0, 30, temp.c_str());
    temp = "Saved animals count: " + std::to_string(savedAnimals);
    tempText.init(Engine::getInstance()->getRenderer(), 0, 0, temp.c_str());
}

void Play::gameUpdate(){
    Menu::getInstance()->checkMenu(Engine::getInstance()->getRenderer());
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

    if (index != -1){
        SDL_Rect animalRect = animals[index]->getBox();
        if (SDL_HasIntersection(&playerRect, &animalRect)) {
            animals[index]->setX(player->getOrigin()->x - 15);
            animals[index]->setY(player->getOrigin()->y - 30);
            if (animalRect.x > 604 && animalRect.x < 800 && animalRect.y > 2200) {
                savedAnimals++;
                animals.erase(animals.begin() + index);
                animals.push_back(Play::getInstance()->renderAnimal());
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
            SoundManager::getInstance()->playEffect("hurt");
        }
    }

    if (player->getHealth() == 2){
        gameOver();
    }


    Camera::getInstance()->update(dt);

    levelMap->update();

    if (savedAnimals >= 4){
        MapParser::getInstance()->clean();
        gameClean();
        Engine::getInstance()->setLevel(2);
    }
}

void Play::gameClean(){
    if (player != nullptr){
        player->clean();
        player = nullptr;
    }
    for(int i = 0; i < animals.size(); i++) animals[i]->clean();
    animals.clear();
    for (int i = 0; i != enemies.size(); i++) enemies[i]->clean();
    enemies.clear();
}

void Play::gameOver(){
    Leaderboard::getInstance()->addToFile(player->getName(), savedAnimals);
    exit(EXIT_SUCCESS);
}

Animals *Play::renderAnimal(){
    x +=50;
    switch (rand()%(11-0)+0) {
        case 0:
            return new Animals(new Properties("bird", x, 2000, 32, 32));
        case 1:
            return new Animals(new Properties("cat1", x, 2000, 48, 48));
        case 2:
            return new Animals(new Properties("cat2", x, 2000, 48, 48));
        case 3:
            return new Animals(new Properties("chicken", x, 2000, 32, 32));
        case 4:
            return new Animals(new Properties("dog1", x, 2000, 48, 48));
        case 5:
            return new Animals(new Properties("dog2", x, 2000, 48, 48));
        case 6:
            return new Animals(new Properties("hyena", x, 2000, 48, 48));
        case 7:
            return new Animals(new Properties("rat", x, 2000, 32, 32));
        case 8:
            return  new Animals(new Properties("scorpion", x, 2000, 48, 48));
        case 9:
            return new Animals(new Properties("snake", x, 2000, 48, 48));
        case 10:
            return new Animals(new Properties("frog", x, 2000, 32, 32));
        default:
            exit(EXIT_FAILURE);
    }
}

std::string Play::getPlayerNameForMenu() {return player->getName();}
