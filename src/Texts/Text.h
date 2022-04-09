//
// Created by TeamsInsane on 4. 03. 2022.
//

#ifndef ANIMALGAME_TEXT_H
#define ANIMALGAME_TEXT_H

#include <SDL_ttf.h>
#include <SDL.h>

class Text {
private:
    SDL_Renderer *renderer;
    SDL_Texture *message;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect messageRect;
public:
    void init(SDL_Renderer *renderer, int x, int y, const char text[], bool subtract = true);
    void initCenter(SDL_Renderer *renderer, int y, int size, char text[]);

    void draw();
    void changeText(char text[]);
    void changeTextCenter(char text[]);
};


#endif //ANIMALGAME_TEXT_H
