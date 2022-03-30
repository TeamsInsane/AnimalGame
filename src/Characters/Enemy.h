//
// Created by TeamsInsane on 24. 02. 2022.
//

#ifndef ANIMALGAME_ENEMY_H
#define ANIMALGAME_ENEMY_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animations/SpriteAnimation.h"
#include "../Physics/Collider.h"

class Enemy : public Character {
private:
    Collider *collider;
    RigidBody *rigidBody;
    SpriteAnimation *animation;
    Vector2D lastSafePosition;

    int moveCount;
    bool moveLeft;
public:
    Enemy(Properties *properties);

    virtual void draw();
    virtual void clean();
    virtual void update(float dt);

    SDL_Rect getBox();
};


#endif //ANIMALGAME_ENEMY_H
