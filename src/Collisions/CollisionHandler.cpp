//
// Created by TeamsInsane on 23. 02. 2022.
//

#include "CollisionHandler.h"
#include "../Core/Engine.h"

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

bool CollisionHandler::checkCollision(SDL_Rect a, SDL_Rect b) {
    bool overLapX = (a.x < b.x + b.w) && (a.x + a.w > b.x);
    bool overLapY = (a.y < b.y + b.h) && (a.y + a.h > b.y);

    return (overLapX && overLapY);
}

bool CollisionHandler::mapCollision(SDL_Rect a) {

    if (a.x < 0 || a.x + a.w >= mapWidth * mapTileSize || a.y < 0 || a.y + a.h >= mapHeight * mapTileSize)
        return true;

    int leftTile = a.x/mapTileSize;
    int rightTile = (a.x + a.w)/mapTileSize;

    int topTile = a.y / mapTileSize;
    int bottomTile = (a.y + a.h) / mapTileSize;

    if (leftTile < 0) leftTile = 0;
    if (rightTile > mapWidth) rightTile = mapWidth;

    if (topTile < 0) topTile = 0;
    if (bottomTile > mapHeight) bottomTile = mapHeight;

    for(int i = leftTile; i <= rightTile; ++i){
        for(int j = topTile; j <= bottomTile; j++){
            if (collisionTileMap[j][i] > 0) return true;
        }
    }
    return false;
}

CollisionHandler::CollisionHandler() {

}