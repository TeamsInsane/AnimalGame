//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_WARRIOR_H
#define ANIMALGAME_WARRIOR_H

#include "Character.h"
#include "../Animations/SpriteAnimation.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Physics/Vector2D.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0
#define RUN_FORCE 10.0f
#define ATTACK_TIME 20.0f

class Warrior: public Character{
private:
    SpriteAnimation *animation;
    RigidBody *rigidBody;
    Collider *collider;
    Vector2D lastSafePosition;

    bool isJumping;
    bool isGrounded;
    bool isFalling;
    bool isRunning;
    bool isAttacking;
    bool isCrouching;
    bool isSpawned;

    float jumpTime;
    float jumpForce;
    float attackTime;
public:
    Warrior(Properties* properties);
    virtual void draw();
    virtual void clean();
    virtual void update(float dt);
    void animationState();

    bool getIsJumpingOrFalling(){
        if (isJumping || isFalling) return true;
        return false;
    }

    SDL_Rect getBox();
};


#endif //ANIMALGAME_WARRIOR_H
