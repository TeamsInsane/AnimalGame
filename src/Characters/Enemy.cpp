//
// Created by TeamsInsane on 24. 02. 2022.
//

#include "Enemy.h"
#include "../Collisions/CollisionHandler.h"
#include "../Cameras/Camera.h"

Enemy::Enemy(Properties *properties) : Character(properties) {

    rigidBody = new RigidBody();
    rigidBody->setGravity(3.0f);

    collider = new Collider();
    collider->setBuffer(-90, -85, 90, 10);

    animation = new SeqAnimation(false);
    animation->parse("../assets/animation.aml");
    animation->setCurrentSeq("wizard_idle");
}

void Enemy::draw() {
    animation->drawFrame(transform->x, transform->y, 1, 1, flip);

    //collider->drawBox();
}

void Enemy::clean() {

}

void Enemy::update(float dt) {
    rigidBody->update(dt);
    lastSafePosition.x = transform->x;
    transform->x += rigidBody->getPosition().x;
    collider->set(transform->x, transform->y, 140, 100);

    if (CollisionHandler::getInstance()->mapCollision(collider->getBox())) transform->x = lastSafePosition.x;

    animation->update(dt);

    lastSafePosition.y = transform->y;
    transform->y += rigidBody->getPosition().y;
    collider->set(transform->x, transform->y, 140, 100);

    if (CollisionHandler::getInstance()->mapCollision(collider->getBox())) transform->y = lastSafePosition.y;

    if (animation->getIsEnded()){
        animation->setRepeat(true);
        animation->setCurrentSeq("wizard_idle");
    }
}

SDL_Rect Enemy::getBox(){
    return collider->getBox();
}
