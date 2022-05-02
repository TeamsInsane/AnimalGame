//
// Created by TeamsInsane on 4. 04. 2022.
//

#ifndef ANIMALGAME_LEADERBOARD_H
#define ANIMALGAME_LEADERBOARD_H

#include <map>
#include <string>

#include "../Texts/Text.h"

class Leaderboard {
private:
    static Leaderboard *instance;
    std::map<std::string, int> map;
    int leaderboardCount;
    Text leaderboard[6];
    SDL_Texture *background;
    SDL_Rect bgRect;
    SDL_Renderer *renderer;
public:
    static Leaderboard *getInstance();

    void updateMap();
    void addToFile(const std::string& name, int score);
    void init(SDL_Renderer *sdlRenderer);

    void draw();
};


#endif //ANIMALGAME_LEADERBOARD_H
