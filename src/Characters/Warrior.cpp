//
// Created by TeamsInsane on 21. 02. 2022.
//

#include "Warrior.h"
#include "../Graphics/TextureManager.h"
#include "SDL.h"
#include "../Inputs/Input.h"
#include "../Cameras/Camera.h"
#include "../Collisions/CollisionHandler.h"
#include "../Factory/ObjectFactory.h"

#define FORWARD 1
#define BACKWARD -1

static Registrar<Warrior> registrar("PLAYER");

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

    animation = new SpriteAnimation();
    animation->setProperties(textureID, 1, 6, 100);
}

void Warrior::draw() {
    animation->draw(transform->x, transform->y, width, height, 1, 1, flip);

    collider->drawBox();
}

void Warrior::clean() {
    TextureManager::getInstance()->dropTexture(textureID);
}

void Warrior::update(float dt) {
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
        rigidBody->applyForceY(UPWARD*jumpForce);
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
    //Idling
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