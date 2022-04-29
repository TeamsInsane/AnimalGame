//
// Created by TeamsInsane on 23. 02. 2022.
//

#include "CollisionHandler.h"

CollisionHandler *CollisionHandler::instance = nullptr;

void CollisionHandler::setCollisionMap(TileMap tileMap, int tileSize){
    collisionTileMap = tileMap;
    mapTileSize = tileSize;
    mapHeight = tileMap.size();
    mapWidth = tileMap[0].size();
}

CollisionHandler *CollisionHandler::getInstance() {
    if (instance == nullptr) instance = new CollisionHandler();
    return instance;
}

bool CollisionHandler::mapCollision(SDL_Rect a) {

    if (a.x < 0 || a.x + a.w >= mapWidth * mapTileSize || a.y < 0 || a.y + a.h >= mapHeight * mapTileSize)
        return true;

    unsigned int leftTile = a.x / mapTileSize;
    unsigned int rightTile = (a.x + a.w) / mapTileSize;

    unsigned int topTile = a.y / mapTileSize;
    unsigned int bottomTile = (a.y + a.h) / mapTileSize;

    if (leftTile < 0) leftTile = 0;
    if (rightTile > mapWidth) rightTile = mapWidth;

    if (topTile < 0) topTile = 0;
    if (bottomTile > mapHeight) bottomTile = mapHeight;

    for (unsigned int i = leftTile; i <= rightTile; ++i) {
        for (unsigned int j = topTile; j <= bottomTile; j++) {
            if (collisionTileMap[j][i] > 0) return true;
        }
    }
    return false;
}