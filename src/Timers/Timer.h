//
// Created by TeamsInsane on 22. 02. 2022.
//

#ifndef ANIMALGAME_TIMER_H
#define ANIMALGAME_TIMER_H

#include <SDL.h>
#include "../Texts/Text.h"

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 0.50f;

class Timer {
private:
    static Timer* instance;
    float deltaTime, lastTime;
    Text time;
public:
    static Timer* getInstance();
    void tick();
    float getDeltaTime() const;

    void displayTime();
};


#endif //ANIMALGAME_TIMER_H
