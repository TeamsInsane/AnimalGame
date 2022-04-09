//
// Created by TeamsInsane on 4. 03. 2022.
//

#ifndef ANIMALGAME_MENU_H
#define ANIMALGAME_MENU_H

#include "../Texts/Text.h"
#include <SDL.h>

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

    int delayCount;

    Text title, options[5], name;
    int position;
public:
    void init(SDL_Renderer *renderer);
    void draw(SDL_Renderer *renderer);

    void moveDown();
    void moveUp();
    int getPosition();

    static Menu *getInstance();
    void update();

    bool getDisplayGame() const;

    void checkMenu(SDL_Renderer *renderer);

    bool getDisplayDirections() const;

    void loadDirections(SDL_Renderer *renderer);

    bool getDisplayLeaderboard() const;
    bool getDisplayReplay() const;

    void setDisplayLeaderboard(bool boolean);

    bool getDisplayMenu() const;

    bool getDisplayGameOver() const;

    void resetMenu(SDL_Renderer *renderer);

    void resetDisplayGame();
};


#endif //ANIMALGAME_MENU_H
