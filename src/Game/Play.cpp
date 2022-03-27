//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Play.h"
#include "../Maps/MapParser.h"
#include "../Graphics/TextureManager.h"
#include "../Sound/SoundManager.h"

Play *Play::instance = nullptr;

void Play::mainGame(GameMap *&levelMap, std::vector<ImgLayer*> &parallaxBg, Warrior *&player, std::vector<Enemy*> &enemies, std::vector<Animals*> &animals){
    if (!MapParser::getInstance()->load("../assets/maps/map2.tmx")){
        SDL_Log("failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SoundManager::getInstance()->parseSounds("../assets/sounds.tml")){
        SDL_Log("failed to parse sounds: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    levelMap = MapParser::getInstance()->getMaps("MAP");

    TileLayer *collisionLayer = (TileLayer*) levelMap->getMapLayers().back();

    parallaxBg.push_back(new ImgLayer("bg", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.2, 2, 2));

    int tileSize = collisionLayer->getTileSize();
    int width = collisionLayer->getWidth()*tileSize;
    int height = collisionLayer->getHeight()*tileSize;

    Camera::getInstance()->setSceneLimit(width, height);

    CollisionHandler::getInstance()->setCollisionMap(collisionLayer->getTileMap(), tileSize);

    TextureManager::getInstance()->parseTextures("../assets/textures.tml");

    player = new Warrior(new Properties("player", 100, 1800, 32, 32));

    for(int i = 0; i < 9; i++) animals.push_back(renderAnimal());

    enemies.push_back(new Enemy(new Properties("boss_idle", 400, 1700, 250, 250)));
    Camera::getInstance()->setTarget(player->getOrigin());

    SoundManager::getInstance()->playMusic("banger");
}

Play *Play::getInstance() {
    if (instance == nullptr) instance = new Play();
    return instance;
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