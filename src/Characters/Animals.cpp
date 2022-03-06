//
// Created by TeamsInsane on 3. 03. 2022.
//

#include "Animals.h"
#include "../Factory/ObjectFactory.h"
#include "../Graphics/TextureManager.h"

static Registrar<Animals> registrar("ANIMAL");

Animals::Animals(Properties *properties) : Character(properties) {

    rigidBody = new RigidBody;
    rigidBody->setGravity(3.0f);

    collider = new Collider();
    collider->setBuffer(0, -13, 100, 65);

    animation = new SpriteAnimation();
    animation->setProperties(textureID, 1, 4, 100);
}

void Animals::draw() {
    animation->draw(transform->x, transform->y, width,height, 1, 1, flip);

    //collider->drawBox();
}

void Animals::clean() {
    TextureManager::getInstance()->dropTexture(textureID);
}

void Animals::update(float dt) {
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
}

SDL_Rect Animals::getBox(){
    return collider->getBox();
}