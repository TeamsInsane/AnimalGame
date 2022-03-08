//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Text.h"

#include "../Core/Engine.h"

void Text::init(SDL_Renderer *renderer, int x, int y, const char *text) {

    this->renderer = renderer;

    font = TTF_OpenFont("../assets/fonts/OpenSans.ttf", 40);
    if (font == nullptr) {
        SDL_Log("Error: font not found!");
        exit(EXIT_FAILURE);
    }

    color = {255, 255, 255, 0};

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);

    message = SDL_CreateTextureFromSurface(this->renderer, surfaceMessage);
    messageRect = {x, y, surfaceMessage->w - 150, surfaceMessage->h - 20};

    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(font);

    SDL_RenderCopyEx(renderer, message, nullptr, &messageRect, 0, nullptr, SDL_FLIP_NONE);

    SDL_DestroyTexture(message);
}

void Text::initCenter(SDL_Renderer *renderer, int y, int size, char *text) {
    this->renderer = renderer;

    font = TTF_OpenFont("../assets/fonts/Planes_ValMore.ttf", size);
    color = {255, 0, 102, 0};

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);

    message = SDL_CreateTextureFromSurface(this->renderer, surfaceMessage);

    messageRect = {(SCREEN_WIDTH - surfaceMessage->w) / 2, y, surfaceMessage->w + 10, surfaceMessage->h + 10};

    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(font);

}

void Text::draw() {
    SDL_RenderCopyEx(renderer, message, nullptr, &messageRect, 0, nullptr, SDL_FLIP_NONE);
}

void Text::changeText(char text[]) {
    SDL_Surface  *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect.w = surfaceMessage->w;
}

void Text::changeTextCenter(char *text) {
    SDL_Surface  *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    messageRect.w = surfaceMessage->w;

    messageRect.x = (SCREEN_WIDTH - surfaceMessage->w) / 2;
}

