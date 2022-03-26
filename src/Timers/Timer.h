//
// Created by TeamsInsane on 22. 02. 2022.
//

#ifndef ANIMALGAME_TIMER_H
#define ANIMALGAME_TIMER_H

#include <SDL.h>

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 0.60f;

class Timer {
private:
    static Timer* instance;
    float deltaTime, lastTime;
public:
    static Timer* getInstance();
    void tick();
    float getDeltaTime() const;
};


#endif //ANIMALGAME_TIMER_H
