//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_COLLISIONHANDLER_H
#define ANIMALGAME_COLLISIONHANDLER_H

#include "../Maps/TileLayer.h"
#include "../Maps/GameMap.h"
#include "../Core/Engine.h"
#include <SDL.h>
#include <vector>

class CollisionHandler {
private:
    static CollisionHandler *instance;

    unsigned int mapTileSize, mapHeight, mapWidth;

    TileMap collisionTileMap;
public:
    static  CollisionHandler *getInstance();

    bool mapCollision(SDL_Rect a);
    void setCollisionMap(TileMap tileMap, int tileSize);
};


#endif //ANIMALGAME_COLLISIONHANDLER_H
