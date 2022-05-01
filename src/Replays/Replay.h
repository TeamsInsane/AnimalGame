//
// Created by Uporabnik on 5. 04. 2022.
//

#ifndef ANIMALGAME_REPLAY_H
#define ANIMALGAME_REPLAY_H

#include "../Characters/Warrior.h"
#include <SDL_render.h>

struct Position{
    float x, y;
};

class Replay {
private:
    static Replay *instance;
    Warrior *player;
    GameMap *levelMap;
    std::vector<ImgLayer*> parallaxBg;
    float safeX, safeY;
    SDL_Renderer *renderer;
    unsigned int readCount;
    Text replayText;
    int initialized;
    int lvl;
public:
    static Replay *getInstance();

    void saveMovement();
    void displayMovement();

    void initReplay(SDL_Renderer *sdlRenderer);

    void render();
    void update();
    void clean();

    void resetReadCount();
};


#endif //ANIMALGAME_REPLAY_H
