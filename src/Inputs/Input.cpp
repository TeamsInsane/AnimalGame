//
// Created by TeamsInsane on 21. 02. 2022.
//

#include "Input.h"
#include "../Core/Engine.h"

Input* Input::instance = nullptr;

Input::Input(){
    capital = false;
    delay = 0;
    keyStates = SDL_GetKeyboardState(nullptr);
}

Input *Input::getInstance() {
    if (instance == nullptr) instance = new Input();
    return instance;
}

void Input::listen() {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                Engine::getInstance()->quit();
                break;
            case SDL_KEYDOWN:
                keyDown();
                break;
            case SDL_KEYUP:
                keyUp();
                break;
        }
    }
}

bool Input::getKeyDown(SDL_Scancode key){
    if(keyStates[key] == 1)
        return true;
    return false;
}

void Input::enterName(std::string &name){
    delay++;
    capital = false;
    char add = ' ';
    if (getKeyDown(SDL_SCANCODE_LSHIFT)) capital = true;
    if (delay > 20) {
        if (getKeyDown(SDL_SCANCODE_A)){
            add = 'a';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_B)){
            add = 'b';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_C)){
            add = 'c';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_D)){
            add = 'd';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_E)){
            add = 'e';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_F)){
            add = 'f';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_G)){
            add = 'g';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_H)){
            add = 'h';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_I)){
            add = 'i';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_J)){
            add = 'j';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_K)){
            add = 'k';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_L)){
            add = 'l';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_M)){
            add = 'm';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_N)){
            add = 'n';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_O)){
            add = 'o';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_P)){
            add = 'p';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_R)){
            add = 'r';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_S)){
            add = 's';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_T)){
            add = 't';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_U)){
            add = 'u';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_V)){
            add = 'v';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_Z)){
            add = 'z';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_Q)){
            add = 'q';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_Y)) {
            add = 'y';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_X)) {
            add = 'x';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_W)){
            add = 'w';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_1)){
            add = '1';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_2)){
            add = '2';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_3)){
            add = '3';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_4)){
            add = '4';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_5)){
            add = '5';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_6)){
            add = '6';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_7)){
            add = '7';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_8)){
            add = '8';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_9)){
            add = '9';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_0)){
            add = '0';
            delay = 0;
        } else if (getKeyDown(SDL_SCANCODE_BACKSPACE)){
            if(!name.empty()) name.pop_back();
            delay = 0;
        }
        if (add != ' ') {
            if (capital) add = toupper(add);
            name += add;
        }
    }
}

void Input::keyUp() {
    keyStates = SDL_GetKeyboardState(nullptr);
}

void Input::keyDown() {
    keyStates = SDL_GetKeyboardState(nullptr);
}

//Return 1 or -1 according to the direction of the key pressed or 0 if nothing is pressed
int Input::getAxisKey(Axis axis) {
    switch (axis) {
        case HORIZONTAL:
            if (getKeyDown(SDL_SCANCODE_D) || getKeyDown(SDL_SCANCODE_RIGHT)) return 1;
            if (getKeyDown(SDL_SCANCODE_A) || getKeyDown(SDL_SCANCODE_LEFT)) return -1;
            break;
        case VERTICAL:
            if (getKeyDown(SDL_SCANCODE_W) || getKeyDown(SDL_SCANCODE_UP)) return 1;
            if (getKeyDown(SDL_SCANCODE_S) || getKeyDown(SDL_SCANCODE_DOWN)) return -1;
            break;
        default:
            return 0;
    }
    return 0;
}
