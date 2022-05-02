//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_CHARACTER_H
#define ANIMALGAME_CHARACTER_H

#include <string>
#include <SDL.h>
#include "../Physics/Point.h"
#include "../Physics/Transform.h"

class Properties{
public:
    Properties(){}
    Properties(std::string textureID, float x, float y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE){
        this->x = x;
        this->y = y;
        this->flip = flip;
        this->width = width;
        this->height = height;
        this->textureID = std::move(textureID);
    }

    std::string textureID;
    int width, height;
    float x, y;
    SDL_RendererFlip flip;
};

class Character: public Properties{
public:
    Character(Properties *properties):  textureID(properties->textureID), width(properties->width), height(properties->height), flip(properties->flip){
        transform = new Transform(properties->x, properties->y);
        float px = properties->x + properties->width/2;
        float py = properties->y + properties->height/2;
        origin = new Point(px, py);
    }
    Point *getOrigin() {return origin;}
    virtual void draw() = 0;
    virtual void clean() = 0;
    virtual void update(float dt) = 0;
protected:
    std::string name;
    Point *origin;
    Transform* transform;
    int width, height;
    std::string textureID;
    SDL_RendererFlip flip;
};

#endif //ANIMALGAME_CHARACTER_H
