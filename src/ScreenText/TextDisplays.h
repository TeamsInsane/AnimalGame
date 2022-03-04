//
// Created by Uporabnik on 3. 03. 2022.
//

#ifndef ANIMALGAME_TEXTDISPLAYS_H
#define ANIMALGAME_TEXTDISPLAYS_H


class TextDisplays {
private:
    static TextDisplays *instance;
    SDL_Renderer* renderer;
public:

    static TextDisplays *getInstance();
    void draw(float x, float y);
};


#endif //ANIMALGAME_TEXTDISPLAYS_H
