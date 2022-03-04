//
// Created by Uporabnik on 4. 03. 2022.
//

#ifndef ANIMALGAME_MENU_H
#define ANIMALGAME_MENU_H

#include "Text.h"
#include <SDL.h>

class Menu {
private:
    static Menu *instance;
    SDL_Texture *background, *arrow;
    SDL_Rect bgRect, arrowRect;

    bool moveArrow;
    bool displayGame;
    bool displayDirections;
    int retard;

    Text title, options[3];
    int position;
public:
    void init(SDL_Renderer *&renderer);
    void draw(SDL_Renderer *&renderer);

    void moveDown();
    void moveUp();
    int getPosition();

    static Menu *getInstance();
    void update();

    bool getDisplayGame() const;

    void checkMenu(SDL_Renderer *renderer);

    bool getDisplayDirections() const;
};


#endif //ANIMALGAME_MENU_H
