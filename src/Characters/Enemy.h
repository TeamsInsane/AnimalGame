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

    void draw() override;
    void clean() override;
    void update(float dt) override;

    SDL_Rect getBox() const;
    bool getMoveLeft() const;
    int getMoveCount() const;

    void setY(float y);
    void setX(float x);
    void setMove(int num, bool left);
};


#endif //ANIMALGAME_ENEMY_H
