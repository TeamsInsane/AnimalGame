//
// Created by TeamsInsane on 24. 02. 2022.
//

#include "Enemy.h"
#include "../Graphics/TextureManager.h"

Enemy::Enemy(Properties *properties) : Character(properties) {

    rigidBody = new RigidBody();
    rigidBody->setGravity(3.0f);

    collider = new Collider();
    animation = new SpriteAnimation();

    animation->setProperties(textureID, 1, 4, 100);
    collider->setBuffer(0, 0, 110, 65);

    moveCount = 0;
    moveLeft = false;
}

void Enemy::draw() {
    animation->draw(transform->x, transform->y, width,height, 1, 1, flip);

    //collider->drawBox();
}

void Enemy::clean() {
    TextureManager::getInstance()->dropTexture(textureID);
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

    origin->x = transform->x + width / 2;
    origin->y = transform->y + height / 2;

    animation->update(dt);


    if (moveCount < 200 && !moveLeft){
        moveCount++;
        transform->x++;
    } else {
        if (moveCount == 0) moveLeft = false;
        else moveLeft = true;
        moveCount--;
        transform->x--;
    }
}

SDL_Rect Enemy::getBox() const{
    return collider->getBox();
}

int Enemy::getMoveCount() const{
    return moveCount;
}

bool Enemy::getMoveLeft() const{
    return moveLeft;
}

void Enemy::setX(float x){
    transform->x = x;
}

void Enemy::setY(float y){
    transform->y = y;
}

void Enemy::setMove(int num, bool left){
    moveCount = num;
    moveLeft = left;
}