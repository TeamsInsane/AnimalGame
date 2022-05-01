//
// Created by TeamsInsane on 3. 03. 2022.
//

#ifndef ANIMALGAME_ANIMALS_H
#define ANIMALGAME_ANIMALS_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animations/SpriteAnimation.h"
#include "../Physics/Collider.h"

class Animals : public Character {
private:
    Collider *collider;
    RigidBody *rigidBody;
    SpriteAnimation *animation;
    Vector2D lastSafePosition;
public:
    Animals(Properties *properties);

    void draw() override;
    void clean() override;
    void update(float dt) override;

    SDL_Rect getBox();

    void setY(float y);
    void setX(float x);
};


#endif //ANIMALGAME_ANIMALS_H
