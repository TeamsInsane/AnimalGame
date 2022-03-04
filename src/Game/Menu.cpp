//
// Created by Uporabnik on 4. 03. 2022.
//

#include "Menu.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Graphics/TextureManager.h"

Menu *Menu::instance = nullptr;

void Menu::init(SDL_Renderer *&renderer) {

    position = 0;

    bgRect = {-500, -1080 + SCREEN_HEIGHT + 100, 1920, 1080};
    SDL_Surface *surface = IMG_Load("../assets/images/Background.png");
    background = SDL_CreateTextureFromSurface(renderer, surface);

    char tempText[60] = "Minecraft 2.0";
    title.initCenter(renderer, SCREEN_HEIGHT / 2 - 150, 70, tempText);

    strcpy(tempText, "Nova igra");
    options[0].initCenter(renderer, SCREEN_HEIGHT / 2 + 50, 40, tempText);

    strcpy(tempText, "Navodila");
    options[1].initCenter(renderer, SCREEN_HEIGHT / 2 + 100, 40, tempText);

    strcpy(tempText, "Izhod");
    options[2].initCenter(renderer, SCREEN_HEIGHT / 2 + 150, 40, tempText);

    arrowRect = {SCREEN_WIDTH - 400, SCREEN_HEIGHT / 2 + 50 - 5, 75, 50};

    surface = IMG_Load("../assets/menu/arrow.png");
    arrow = SDL_CreateTextureFromSurface(renderer, surface);
}

void Menu::draw(SDL_Renderer *&renderer) {
    SDL_RenderCopyEx(renderer, background, nullptr, &bgRect, 0, nullptr, SDL_FLIP_NONE);
    title.draw();
    for (int i = 0; i<3; i++) options[i].draw();
    SDL_RenderCopyEx(renderer, arrow, nullptr, &arrowRect, 0, nullptr, SDL_FLIP_NONE);
}

void Menu::moveDown() {
    if (position != 2){
        position++;
        arrowRect.y += 50;
    }else{
        position = 0;
        arrowRect.y = SCREEN_HEIGHT / 2 + 50 - 5;
    }
}

void Menu::moveUp() {
    if (position != 0)
    {
        position--;
        arrowRect.y -= 50;
    }
    else
    {
        position = 2;
        arrowRect.y = SCREEN_HEIGHT / 2 + 150 - 5;
    }
}

int Menu::getPosition() {
    return position;
}

Menu *Menu::getInstance() {
    if (instance == nullptr) instance = new Menu();
    return instance;
}

void Menu::update() {
    retard++;
    if (retard == 49){
        retard = 0;
        moveArrow = true;
    }
    if (!moveArrow) return;

    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_UP)) {
        moveUp();
        moveArrow = false;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_DOWN)) {
        moveDown();
        moveArrow = false;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 0) {
        displayGame = true;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 1) {
        displayDirections = true;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 2) {
        SDL_DestroyRenderer(Engine::getInstance()->getRenderer());
        SDL_DestroyWindow(Engine::getInstance()->getWindow());
        IMG_Quit();
        SDL_Quit();
        Engine::getInstance()->quit();
    }

}

void Menu::checkMenu(SDL_Renderer *renderer){
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_ESCAPE)) {
        if (displayGame){
            char tempText[20] = "Nadaljuj";
            options[0].initCenter(renderer, SCREEN_HEIGHT / 2 + 50, 40, tempText);
        }
        displayGame = false;
        displayDirections = false;
    }
}

bool Menu::getDisplayGame() const{return displayGame;}

bool Menu::getDisplayDirections() const {return displayDirections;}

void Menu::loadDirections(SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load("../assets/menu/navodila.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {0, 0, 960, 640};
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, 0, nullptr, SDL_FLIP_NONE);
}