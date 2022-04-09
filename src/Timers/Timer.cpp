//
// Created by TeamsInsane on 22. 02. 2022.
//

#include "Timer.h"
#include "../Core/Engine.h"
#include "../Game/Play.h"
#include <string>

Timer* Timer::instance = nullptr;

Timer *Timer::getInstance() {
    if (instance == nullptr) instance = new Timer();
    return instance;
}

void Timer::tick(){
    deltaTime = (SDL_GetTicks() - lastTime) * (TARGET_FPS / 1000.0f);

    if (deltaTime > TARGET_DELTATIME) deltaTime = TARGET_DELTATIME;

    lastTime = SDL_GetTicks();
}

void Timer::displayTime() {
    int seconds = SDL_GetTicks()/1000 - Play::getInstance()->getStartTime();
    int minutes = 0;
    while (seconds >= 60){
        minutes++;
        seconds -=60;
    }
    std::string tempText = "Time: " + std::to_string(minutes) + ":" + std::to_string(seconds);
    time.init(Engine::getInstance()->getRenderer(), 0, 60, tempText.c_str(), false);
}

float Timer::getDeltaTime() const{
    return deltaTime;
}