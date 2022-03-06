//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_COLLIDER_H
#define ANIMALGAME_COLLIDER_H

#include <SDL.h>
#include "../Cameras/Camera.h"
#include "../Collisions/CollisionHandler.h"

class Collider{
private:
    SDL_Rect box;
    SDL_Rect buffer;
public:
    SDL_Rect getBox(){return box;}
    void setBuffer(int x, int y, int w, int h){buffer = {x, y, w, h};}

    void set(int x, int y, int w, int h){
        box = {
                x - buffer.x,
                y - buffer.y,
                w - buffer.w,
                h - buffer.h
        };
    }

    void drawBox(){
        Vector2D cam = Camera::getInstance()->getPosition();
        SDL_Rect box = {this->box.x - (int) cam.x, this->box.y - (int)  cam.y, this->box.w, this->box.h};
        SDL_RenderDrawRect(Engine::getInstance()->getRenderer(), &box);
    }
};

#endif //ANIMALGAME_COLLIDER_H
