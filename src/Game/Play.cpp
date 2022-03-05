//
// Created by Uporabnik on 4. 03. 2022.
//

#include "Play.h"
#include "../Maps/MapParser.h"
#include "../Graphics/TextureManager.h"
#include "../Factory/ObjectFactory.h"
#include "../Sound/SoundManager.h"

Play *Play::instance = nullptr;

void Play::mainGame(GameMap *&levelMap, std::vector<ImgLayer*> &parallaxBg, Warrior *&player, std::vector<GameObject*> &gameObject){
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

    player = new Warrior(new Properties("player", 100, 200, 32, 32));

    //Properties *playerProperties = new Properties("player", 100, 200, 32, 32);
    //GameObject *player = ObjectFactory::getInstance()->createObject("PLAYER", playerProperties);

    Properties *animalProperties = new Properties("animal_idle", 150, 200, 48, 48);
    GameObject *animal = ObjectFactory::getInstance()->createObject("ANIMAL", animalProperties);

    Properties *bossProperties = new Properties("boss_idle", 400, 100, 250, 250);
    GameObject *boss = ObjectFactory::getInstance()->createObject("ENEMY", bossProperties);

    gameObject.push_back(player);
    gameObject.push_back(boss);
    gameObject.push_back(animal);
    Camera::getInstance()->setTarget(player->getOrigin());

    SoundManager::getInstance()->playMusic("banger");
}

Play *Play::getInstance() {
    if (instance == nullptr) instance = new Play();
    return instance;
}
