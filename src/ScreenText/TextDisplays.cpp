//
// Created by Uporabnik on 3. 03. 2022.
//

#include <SDL_ttf.h>
#include "TextDisplays.h"
#include "../Core/Engine.h"

TextDisplays *TextDisplays::instance = nullptr;

TextDisplays *TextDisplays::getInstance() {
    if (instance == nullptr) instance = new TextDisplays;
    return instance;
}

void TextDisplays::draw(float x, float y) {

    renderer = Engine::getInstance()->getRenderer();

    TTF_Font *sans = TTF_OpenFont("../assets/fonts/lato.ttf", 20);
    if (sans == nullptr) {
        SDL_Log("Error: font not found!");
        exit(EXIT_FAILURE);
    }

    SDL_Color White = {255, 255, 255};

    std::string text = "x: " + std::to_string(x) + " y: " + std::to_string(y);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, text.c_str(), White);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FreeSurface(surfaceMessage);
    surfaceMessage = nullptr;

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 400; // controls the width of the rect
    Message_rect.h = 24; // controls the height of the rect

    SDL_RenderCopyEx(renderer, Message, nullptr, &Message_rect, 0, nullptr, SDL_FLIP_NONE);
}
