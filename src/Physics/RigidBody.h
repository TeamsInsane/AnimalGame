//
// Created by TeamsInsane on 21. 02. 2022.
//

#ifndef ANIMALGAME_RIGIDBODY_H
#define ANIMALGAME_RIGIDBODY_H

#include "Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 17.0f
#define UPWARD -1

class RigidBody{
private:
    float mass, gravity;

    Vector2D force;
    Vector2D friction;

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
public:
    RigidBody(){
        mass = UNI_MASS;
        gravity = GRAVITY;
    }

    //Setters for gravity
    void setGravity(float grav){ gravity = grav;}

    //Functions for force
    void applyForceX(float Fx){ force.x = Fx;}
    void applyForceY (float Fy){ force.y = Fy;}
    void unSetForce(){ force = Vector2D{0, 0};}

    //Getters
    Vector2D getPosition(){return position;}
    Vector2D getVelocity(){return velocity;}

    void update(float dt){
        acceleration.x = (force.x + friction.x) / mass;
        acceleration.y = (gravity + force.y) / mass;
        velocity = acceleration * dt;
        position = velocity * dt;
    }
};

#endif //ANIMALGAME_RIGIDBODY_H
