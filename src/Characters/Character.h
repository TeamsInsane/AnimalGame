//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_CHARACTER_H
#define ANIMALGAME_CHARACTER_H

#include <string>

#include "../Objects/GameObject.h"

class Character: public GameObject{
public:
    Character(Properties *properties): GameObject(properties){}
    virtual void draw() = 0;
    virtual void clean() = 0;
    virtual void update(float dt) = 0;
protected:
    std::string name;
};


#endif //ANIMALGAME_CHARACTER_H
