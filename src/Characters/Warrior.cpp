//
// Created by TeamsInsane on 21. 02. 2022.
//

#include "Warrior.h"
#include "../Graphics/TextureManager.h"
#include "SDL.h"
#include "../Inputs/Input.h"

#define FORWARD 1
#define BACKWARD -1

Warrior::Warrior(Properties* properties): Character(properties) {
    isSpawned = false;
    isGrounded = false;
    jumpTime = JUMP_TIME;
    jumpForce = JUMP_FORCE;
    attackTime = ATTACK_TIME;

    collider = new Collider();
    collider->setBuffer(-10, 4, 78, 60);

    rigidBody = new RigidBody();
    rigidBody->setGravity(400.0f);

    health = 3;

    animation = new SpriteAnimation();
    animation->setProperties(textureID, 1, 6, 100);
    animationStateNumber = 1;
}

void Warrior::draw() {
    animation->draw(transform->x, transform->y, width, height, 1, 1, flip);

    collider->drawBox();
}

void Warrior::clean() {
    TextureManager::getInstance()->dropTexture(textureID);
}

void Warrior::update(float dt) {
    Camera::getInstance()->setTarget(getOrigin());
    if (isGrounded && !isSpawned){
        isSpawned = true;
        rigidBody->setGravity(GRAVITY);
    }
    isRunning = false;
    isCrouching = false;
    rigidBody->unSetForce();

    //Run forward
    if (Input::getInstance()->getAxisKey(HORIZONTAL) == FORWARD && !isAttacking){
        rigidBody->applyForceX(FORWARD*RUN_FORCE);
        flip = SDL_FLIP_NONE;
        isRunning = true;
    }

    //Run backward
    if (Input::getInstance()->getAxisKey(HORIZONTAL) == BACKWARD && !isAttacking){
        rigidBody->applyForceX(BACKWARD*RUN_FORCE);
        flip = SDL_FLIP_HORIZONTAL;
        isRunning = true;
    }

    //Crouch
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_S)){
        rigidBody->unSetForce();
        isCrouching = true;
    }

    //Attack
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_F)){
        rigidBody->unSetForce();
        isAttacking = true;
    }

    //Jump
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && isGrounded){
        isJumping = true;
        isGrounded = false;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && isJumping && jumpTime > 0){
        jumpTime -= dt;
        rigidBody->applyForceY(UPWARD*jumpForce);
    } else {
        isJumping = false;
        jumpTime = JUMP_TIME;
    }

    //Fall
    if (rigidBody->getVelocity().y > 0 && !isGrounded) isFalling = true;
    else isFalling = false;

    //Attack timer
    if (isAttacking && attackTime > 0) attackTime -= dt;
    else {
        isAttacking = false;
        attackTime = ATTACK_TIME;
    }

    //Move on X axis
    rigidBody->update(dt);
    lastSafePosition.x = transform->x;
    transform->x += rigidBody->getPosition().x;
    collider->set(transform->x, transform->y, 96, 96);

    if (CollisionHandler::getInstance()->mapCollision(collider->getBox())) transform->x = lastSafePosition.x;

    //move on Y axis
    rigidBody->update(dt);
    lastSafePosition.y = transform->y;
    transform->y += rigidBody->getPosition().y;
    collider->set(transform->x, transform->y, 96, 96);

    if (CollisionHandler::getInstance()->mapCollision(collider->getBox())){
        isGrounded = true;
        transform->y = lastSafePosition.y;
    } else isGrounded = false;

    origin->x = transform->x + width / 2;
    origin->y = transform->y + height / 2;

    animationState();
    animation->update(dt);
}

void Warrior::animationState() {
    //Idling - 1
    animation->setProperties("player_idle", 1, 4, 100);

    //Running
    if (isRunning) animation->setProperties("player_run", 1, 6, 100);

    //Crouching
    if (isCrouching) animation->setProperties("player_crouch", 1, 4, 100);

    //Jumping
    if (isJumping) animation->setProperties("player_jump", 1, 4, 100);

    //Falling
    if (isFalling) animation->setProperties("player_fall", 1, 4, 100);

    //Attacking
    if (isAttacking) animation->setProperties("player_attack", 1, 6, 100);
}

void Warrior::setAnimationStateNumber(int number){
    switch (number) {
        case 1:
            animationStateNumber = 1;
            break;
        case 2:
            animationStateNumber = 2;
            break;
        case 3:
            animationStateNumber = 3;
            break;
        case 4:
            animationStateNumber = 4;
            break;
        case 5:
            animationStateNumber = 5;
            break;
        case 6:
            animationStateNumber = 6;
            break;
        default:
            SDL_Log("Error, wrong animation state number!");
    }
}

SDL_Rect Warrior::getBox(){
    return collider->getBox();
}

int Warrior::getHealth() const{return health;}

void Warrior::changeHealth(int num) {health = num;}

void Warrior::setX(float x){
    transform->x = x;
}

void Warrior::setY(float y){
    transform->y = y;
}

void Warrior::setCollider(int x, int y, int w, int h){
    collider->set(x, y, w, h);
}

void Warrior::setAnimationSate(float dt){
    animation->setProperties("player_idle", 1, 4, 100);
    animation->update(dt);
}