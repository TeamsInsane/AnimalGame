//
// Created by Uporabnik on 23. 02. 2022.
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
    CollisionHandler();
    static CollisionHandler *instance;

    int mapTileSize, mapHeight, mapWidth;

    TileMap collisionTileMap;
public:
    static  CollisionHandler *getInstance();

    bool checkCollision(SDL_Rect a, SDL_Rect b);
    bool mapCollision(SDL_Rect a);

    void setCollisionMap(TileMap tileMap, int tileSize);
};


#endif //ANIMALGAME_COLLISIONHANDLER_H
