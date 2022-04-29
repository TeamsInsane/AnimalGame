//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_VECTOR2D_H
#define ANIMALGAME_VECTOR2D_H

#include <SDL.h>
#include <string>

class Vector2D {
public:
    float x, y;

    Vector2D(float x = 0, float y = 0) : x(x), y(y) {};
    Vector2D operator+(const Vector2D &v2) const {return Vector2D(x + v2.x, y + v2.y);}
    Vector2D operator-(const Vector2D &v2) const {return Vector2D(x - v2.x, y - v2.y);}
    Vector2D operator*(const float scalar) const {return Vector2D(x*scalar, y*scalar);}
};

#endif //ANIMALGAME_VECTOR2D_H
