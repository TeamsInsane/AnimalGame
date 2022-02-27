//
// Created by Uporabnik on 23. 02. 2022.
//

#include "Camera.h"

Camera *Camera::instance = nullptr;

Camera *Camera::getInstance() {
    if (instance == nullptr) instance = new Camera();
    return instance;
}

void Camera::update(float dt) {

    if (target != nullptr){
        viewBox.x = target->x - SCREEN_WIDTH / 2;
        viewBox.y = target->y - SCREEN_HEIGHT / 2;

        if (viewBox.x < 0) viewBox.x = 0;

        if (viewBox.y < 0) viewBox.y = 0;

        if (viewBox.x > (2 * SCREEN_WIDTH - viewBox.w)) viewBox.x = (2 * SCREEN_WIDTH - viewBox.w);

        if (viewBox.y > (2 * SCREEN_HEIGHT - viewBox.h)) viewBox.y = (2 * SCREEN_HEIGHT - viewBox.h);

        position = Vector2D(viewBox.x, viewBox.y);
    }
}
