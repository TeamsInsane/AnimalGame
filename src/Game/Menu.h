//
// Created by TeamsInsane on 4. 03. 2022.
//

#ifndef ANIMALGAME_MENU_H
#define ANIMALGAME_MENU_H

#include "../Texts/Text.h"
#include <SDL.h>
#include <string>

class Menu {
private:
    static Menu *instance;
    SDL_Texture *background, *arrow;
    SDL_Rect bgRect, arrowRect;

    bool moveArrow;
    bool displayGame;
    bool displayDirections;
    bool displayLeaderboard;
    bool displayMenu;
    bool displayReplay;
    std::string playerName;

    int delayCount;

    Text title, options[7], name;
    int position;
public:
    static Menu *getInstance();

    void init(SDL_Renderer *renderer);
    void draw(SDL_Renderer *renderer);
    void update();
    void checkMenu(SDL_Renderer *renderer);
    void loadDirections(SDL_Renderer *renderer);

    void moveDown();
    void moveUp();

    bool getDisplayDirections() const;
    bool getDisplayLeaderboard() const;
    bool getDisplayReplay() const;
    bool getDisplayGame() const;
    bool getDisplayMenu() const;

    void resetMenu(SDL_Renderer *renderer);
    void resetDisplayGame();

    std::string getName();
};


#endif //ANIMALGAME_MENU_H
