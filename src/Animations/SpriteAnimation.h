//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_SPRITEANIMATION_H
#define ANIMALGAME_SPRITEANIMATION_H

#include <SDL.h>
#include <string>

class SpriteAnimation{
private:
    int animationSpeed, spriteRow, frameCount;
    int currentFrame;
    std::string textureID;
public:
    virtual void update(float dt);
    void draw(float x, float y, int spriteWidth, int spriteHeight, float xScale = 1, float yScale = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void setProperties(std::string textureID, int spriteRow, int frameCount, int animationSpeed);
};


#endif //ANIMALGAME_SPRITEANIMATION_H
