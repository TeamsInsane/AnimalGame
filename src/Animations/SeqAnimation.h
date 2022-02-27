//
// Created by Uporabnik on 23. 02. 2022.
//

#ifndef ANIMALGAME_SEQANIMATION_H
#define ANIMALGAME_SEQANIMATION_H

#include "Animation.h"
#include <SDL.h>
#include <string>
#include <vector>
#include <map>

struct Sequence{
    int sequenceSpeed;
    int frameCount;
    int width, height;
    std::vector<std::string> textureIDs;
};

class SeqAnimation : public Animation{
private:
    Sequence currentSeq;
    std::map<std::string, Sequence> seqMap;
public:
    SeqAnimation(bool repeat = true);

    virtual void update(float dt);
    void parse(std::string source);
    void setCurrentSeq(std::string seqID);
    void setRepeat(bool repeat){
        this->repeat = repeat;
    }
    void drawFrame(float x, float y, float xScale = 1, float yScale = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);
};


#endif //ANIMALGAME_SEQANIMATION_H
