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
#define JUMP_FORCE 40.0f
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
    bool isSpawned;
    bool isHurt;

    std::string name;
    int health;

    float jumpTime;
    float jumpForce;
public:
    Warrior(Properties* properties);
    void draw();
    void clean();
    void update(float dt);
    void animationState();

    bool getIsJumpingOrFalling() const{ return isJumping || isFalling;}

    SDL_Rect getBox();
    int getHealth() const;
    void changeHealth(int num);
    std::string getName(){return name;}
    void setName(std::string text){name = std::move(text);}

    void setY(float y);
    void setX(float x);

    void setCollider(int x, int y, int w, int h);

    void setAnimationSate(float dt);

    void setIsHurt();

    void resetIsHurt();
};


#endif //ANIMALGAME_WARRIOR_H
