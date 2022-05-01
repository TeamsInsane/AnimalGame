//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_INPUT_H
#define ANIMALGAME_INPUT_H

#include <SDL.h>
#include <string>

enum Axis {HORIZONTAL, VERTICAL};

class Input{
private:
    Input();
    static Input* instance;
    int delay;
    bool capital;

    void keyDown();
    void keyUp();

    const Uint8* keyStates;
public:
    static Input* getInstance();

    void listen();
    bool getKeyDown(SDL_Scancode key);
    int getAxisKey(Axis axis);
    void enterName(std::string &name);
};

#endif //ANIMALGAME_INPUT_H
