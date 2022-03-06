//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_ANIMATION_H
#define ANIMALGAME_ANIMATION_H


#include <SDL.h>
#include <string>

class Animation {
protected:
    bool repeat;
    bool isEnded;
    int currentFrame;
public:
    Animation(bool repeat = true) : repeat(repeat){
        isEnded = false;
    }

    virtual void update(float dt) = 0;
    bool getIsEnded(){return isEnded;}
};


#endif //ANIMALGAME_ANIMATION_H
