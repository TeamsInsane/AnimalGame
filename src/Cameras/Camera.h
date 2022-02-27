//
// Created by Uporabnik on 23. 02. 2022.
//

#ifndef ANIMALGAME_CAMERA_H
#define ANIMALGAME_CAMERA_H

#include "../Physics/Point.h"
#include "../Physics/Vector2D.h"
#include "../Core/Engine.h"

class Camera {
private:
    static Camera *instance;
    Camera(){
        viewBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    };
    int sceneWidth, sceneHeight;
    Point *target;
    Vector2D position;
    SDL_Rect viewBox;
public:
    static Camera *getInstance();
    void update(float dt);

    int getSceneWidth() const {return sceneWidth;}
    int getSceneHeight() const {return sceneHeight;}

    void moveX(float x){position.x = x;}
    void moveY(float y){position.y = y;}

    SDL_Rect getViewBox() {return viewBox;}
    Vector2D getPosition() {return position;}
    void setTarget(Point *target){ this->target = target;}
    void setSceneLimit(int w, int h){sceneWidth = w, sceneHeight = h;}
};


#endif //ANIMALGAME_CAMERA_H
