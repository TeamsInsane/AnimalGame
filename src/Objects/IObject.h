//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_IOBJECT_H
#define ANIMALGAME_IOBJECT_H

class IObject{
public:
    virtual void draw() = 0;
    virtual void update(float dt) = 0;
    virtual void clean() = 0;
};

#endif //ANIMALGAME_IOBJECT_H
