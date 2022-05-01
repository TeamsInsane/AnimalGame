//
// Created by Uporabnik on 5. 04. 2022.
//

#include <fstream>
#include "Replay.h"
#include "../Game/Play.h"
#include "../Maps/MapParser.h"
#include "../Graphics/TextureManager.h"
#include "../Sound/SoundManager.h"
#include "../Timers/Timer.h"

Replay *Replay::instance = nullptr;

Replay *Replay::getInstance() {
    if (instance == nullptr) instance = new Replay;
    return instance;
}

void Replay::saveMovement(){
    std::ofstream data("Replay.bin", std::ios::app | std::ios::binary);
    Position position = Position();
    position.x = Play::getInstance()->getPlayerPositionX();
    position.y = Play::getInstance()->getPlayerPositionY();
    data.write((char *) &position, sizeof(position));
    data.close();
}

void Replay::displayMovement(){

    std::ifstream data("Replay.bin", std::ios::binary);
    if (!data.is_open()){
        SDL_Log("Couldn't open or find the data!");
        return;
    }

    data.seekg(sizeof(Position) * readCount, std::ios::beg);
    struct Position position = Position();

    data.read((char *) &position, sizeof (position));
    readCount++;

    position.y -= 10;

    player->setX(position.x);
    player->setY(position.y);
    player->setCollider(position.x, position.y, 96, 96);
    player->getOrigin()->x = position.x + 32 /2;
    player->getOrigin()->y = position.y + 32 /2;

    data.seekg(sizeof(Position) * readCount, std::ios::beg);
    data.read((char *) &position, sizeof (position));

    if (position.x != 0 && position.y != -10.0) {
        safeX = position.x;
        safeY = position.y;
        if (!initialized) {
            SDL_Log("TEMPTEXT -> WATCHING REPLAY");
            char tempText[] = "Watching a replay!";
            replayText.initCenter(renderer, 200, 50, tempText);
            initialized = true;
        }
    } else {
        player->setX(safeX);
        player->setY(safeY);
        player->setCollider(safeX, safeY, 96, 96);
        player->getOrigin()->x = safeX + 32 /2;
        player->getOrigin()->y = safeY + 32 /2;
        if (initialized) {
            SDL_Log("TEMPTEXT -> REPLAY FINISHED");
            char tempText[] = "Replay finished!";
            replayText.initCenter(renderer, 200, 50, tempText);
            initialized = false;
        }
    }

    data.close();
}

void Replay::initReplay(SDL_Renderer *sdlRenderer){
    readCount = 0;

    char tempText[] = "Replay finished!";
    replayText.initCenter(sdlRenderer, 200, 50, tempText);

    this->renderer = sdlRenderer;
    player = new Warrior(new Properties("player", 1500, 1800, 32, 32 ));

    if (!MapParser::getInstance()->load("replay1", "../assets/maps/level1.tmx")){
        SDL_Log("Failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!MapParser::getInstance()->load("replay2", "../assets/maps/level2.tmx")){
        SDL_Log("Failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    std::ifstream data("Replay.txt");
    data >> lvl;
    data.close();

    if (lvl == 1) levelMap = MapParser::getInstance()->getMaps("replay1");
    else levelMap = MapParser::getInstance()->getMaps("replay2");


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
}


void Replay::render(){
    for (int i = 0; i != parallaxBg.size(); i++) parallaxBg[i]->render();
    levelMap->render();
    player->draw();
    replayText.draw();
}

void Replay::update(){
    Menu::getInstance()->checkMenu(renderer);
    float dt = Timer::getInstance()->getDeltaTime();
    displayMovement();
    player->setAnimationSate(dt);
    SoundManager::getInstance()->update();
    Camera::getInstance()->setTarget(player->getOrigin());
    Camera::getInstance()->update();
    levelMap->update();
}

void Replay::clean(){
    delete player;
    delete levelMap;
    parallaxBg.clear();
}

void Replay::resetReadCount(){readCount = 0;}