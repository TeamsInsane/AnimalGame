//
// Created by Uporabnik on 5. 04. 2022.
//

#ifndef ANIMALGAME_REPLAY_H
#define ANIMALGAME_REPLAY_H

#include "../Characters/Warrior.h"
#include <SDL_render.h>

class Replay {
private:
    static Replay *instance;
    Warrior *player;
    GameMap *levelMap;
    std::vector<ImgLayer*> parallaxBg;
    float safeX, safeY;
    SDL_Renderer *renderer;
    int readCount;
    Text replayText;
public:
    Replay();

    static Replay *getInstance();

    void saveMovement();
    void displayMovement();

    void initReplay(SDL_Renderer *renderer);

    void render();

    void update();

    void resetReadCount();
};


#endif //ANIMALGAME_REPLAY_H
