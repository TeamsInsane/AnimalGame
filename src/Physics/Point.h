//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_POINT_H
#define ANIMALGAME_POINT_H

#include <string>
#include <SDL_log.h>

class Point{
public:
    float x, y;

public:
    Point(float x = 0, float y = 0) : x(x), y(y){}

    Point operator+(const Point &p2) const {
        return Point(x + p2.x, y + p2.y);
    }

    friend Point operator+=(Point &p1, const Point &p2){
        p1.x += p2.x;
        p1.y += p2.y;
        return p1;
    }

    Point operator-(const Point &p2) const {
        return Point(x - p2.x, y - p2.y);
    }

    friend Point operator-=(Point &p1, const Point &p2){
        p1.x -= p2.x;
        p1.y -= p2.y;
        return p1;
    }

    Point operator*(const float scalar) const {
        return Point(x * scalar, y * scalar);
    }
};


#endif //ANIMALGAME_POINT_H
