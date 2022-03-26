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

    animation = new SpriteAnimation();
    SDL_Log("texture id - %s", textureID.c_str());
    if (textureID == "frog") {
        animation->setProperties(textureID, 1, 2, 200);
        collider->setBuffer(0, 2, 100, 70);
    }
    else{
        animation->setProperties(textureID, 1, 4, 100);
        collider->setBuffer(0, -13, 100, 65);
    }

    if (textureID == "rat") collider->setBuffer(0, -22, 116, 90);
    else if (textureID == "bird") collider->setBuffer(0, -15, 122, 82);
    else if (textureID == "chicken") collider->setBuffer(0, 1, 110, 70);
    else if (textureID == "hyena") collider->setBuffer(-9, -13, 100, 65);
}

void Animals::draw() {
    animation->draw(transform->x, transform->y, width,height, 1, 1, flip);

    collider->drawBox();
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

void Animals::setX(float x){
    transform->x = x;
}

void Animals::setY(float y){
    transform->y = y;
}