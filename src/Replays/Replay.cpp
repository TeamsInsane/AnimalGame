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

Replay::Replay() {
    std::ofstream data("Replay.bin", std::ios::binary);
    data.close();
}

Replay *Replay::getInstance() {
    if (instance == nullptr) instance = new Replay;
    return instance;
}

struct Position{
    float x, y;
};

void Replay::saveMovement(){
    std::ofstream data("Replay.bin", std::ios::app | std::ios::binary);
    Position position;
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
    struct Position position;

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
        char tempText[] = "Watching a replay!";
        replayText.initCenter(renderer, 200, 50, tempText);
    } else {
        player->setX(safeX);
        player->setY(safeY);
        player->setCollider(safeX, safeY, 96, 96);
        player->getOrigin()->x = safeX + 32 /2;
        player->getOrigin()->y = safeY + 32 /2;
        char tempText[] = "Replay finished!";
        replayText.initCenter(renderer, 200, 50, tempText);
    }

    data.close();
}

void Replay::initReplay(SDL_Renderer *renderer){
    readCount = 0;

    this->renderer = renderer;
    player = new Warrior(new Properties("player", 1500, 1800, 32, 32 ));

    if (!MapParser::getInstance()->load("replay1", "../assets/maps/level1.tmx")){
        SDL_Log("Failed to load map: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    levelMap = MapParser::getInstance()->getMaps("replay1");


    TileLayer *collisionLayer = (TileLayer*) levelMap->getMapLayers().back();


    parallaxBg.push_back(new ImgLayer("bg", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.2, 2, 2));

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
    SoundManager::getInstance()->update(player);
    Camera::getInstance()->setTarget(player->getOrigin());
    Camera::getInstance()->update(dt);
    levelMap->update();
}

void Replay::resetReadCount(){readCount = 0;}