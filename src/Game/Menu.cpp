//
// Created by TeamsInsane on 4. 03. 2022.
//

#include "Menu.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Graphics/TextureManager.h"
#include "Play.h"
#include "../Replays/Replay.h"
#include "../Leaderboards/Leaderboard.h"

Menu *Menu::instance = nullptr;

void Menu::init(SDL_Renderer *renderer) {

    position = 0;

    bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_Surface *surface = IMG_Load("../assets/images/menuBackground.png");
    background = SDL_CreateTextureFromSurface(renderer, surface);

    char tempText[60] = "Minecraft 2.0";
    title.initCenter(renderer, SCREEN_HEIGHT / 2 - 400, 120, tempText);

    strcpy(tempText, "New game");
    options[0].initCenter(renderer, SCREEN_HEIGHT / 2 - 100, 50, tempText);

    strcpy(tempText, "Directions");
    options[1].initCenter(renderer, SCREEN_HEIGHT / 2, 50, tempText);

    strcpy(tempText, "Leaderboard");
    options[2].initCenter(renderer, SCREEN_HEIGHT / 2 + 100, 50, tempText);

    strcpy(tempText, "Replay");
    options[3].initCenter(renderer, SCREEN_HEIGHT / 2 + 200, 50, tempText);

    strcpy(tempText, "Exit");
    options[4].initCenter(renderer, SCREEN_HEIGHT / 2 + 300, 50, tempText);

    strcpy(tempText, "Ime: ");
    name.initCenter(renderer, SCREEN_HEIGHT / 2 + 400, 30, tempText);

    arrowRect = {SCREEN_WIDTH /2 + 220, SCREEN_HEIGHT / 2 - 100, 75, 50};

    surface = IMG_Load("../assets/menu/arrow.png");
    arrow = SDL_CreateTextureFromSurface(renderer, surface);

    displayMenu = true;

    SDL_FreeSurface(surface);
}

void Menu::draw(SDL_Renderer *renderer) {
    SDL_RenderCopyEx(renderer, background, nullptr, &bgRect, 0, nullptr, SDL_FLIP_NONE);
    title.draw();
    name.draw();
    for (int i = 0; i<5; i++) options[i].draw();
    SDL_RenderCopyEx(renderer, arrow, nullptr, &arrowRect, 0, nullptr, SDL_FLIP_NONE);
}

void Menu::moveDown() {
    if (position != 4){
        position++;
        arrowRect.y += 100;
    }else{
        position = 0;
        arrowRect.y = SCREEN_HEIGHT / 2 - 100;
    }
}

void Menu::moveUp() {
    if (position != 0)
    {
        position--;
        arrowRect.y -= 100;
    }
    else
    {
        position = 4;
        arrowRect.y = SCREEN_HEIGHT / 2 + 300;
    }
}

Menu *Menu::getInstance() {
    if (instance == nullptr) instance = new Menu();
    return instance;
}

void Menu::update() {
    delayCount++;
    if (delayCount == 49){
        delayCount = 0;
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
        displayMenu = false;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 1) {
        displayDirections = true;
        displayMenu = false;
    }
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 2) {
        displayLeaderboard = true;
        Leaderboard::getInstance()->init(Engine::getInstance()->getRenderer());
        displayMenu = false;
    }

    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 3) {
        displayReplay = true;
        Replay::getInstance()->resetReadCount();
        displayMenu = false;
    }

    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && position == 4) {
        Engine::getInstance()->clean();
        Engine::getInstance()->quit();
    }
}

void Menu::checkMenu(SDL_Renderer *renderer){
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_ESCAPE)) {
        if (displayGame){
            char tempText[20] = "Nadaljuj";
            options[0].initCenter(renderer, SCREEN_HEIGHT / 2 - 100, 65, tempText);
            strcpy(tempText, "Ime: ");
            strcat(tempText, Play::getInstance()->getPlayerNameForMenu().c_str());
            name.initCenter(renderer, SCREEN_HEIGHT / 2 + 400, 30, tempText);
        }
        displayGame = false;
        displayDirections = false;
        displayLeaderboard = false;
        displayReplay = false;
        displayMenu = true;
    }
}

void Menu::resetMenu(SDL_Renderer *renderer){
    if (Input::getInstance()->getKeyDown(SDL_SCANCODE_ESCAPE)){
        init(renderer);
        Play::getInstance()->resetDisplayGameOver();
    }
}

void Menu::loadDirections(SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load("../assets/menu/navodila.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, 0, nullptr, SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

bool Menu::getDisplayGame() const{return displayGame;}
bool Menu::getDisplayDirections() const {return displayDirections;}
bool Menu::getDisplayLeaderboard() const {return displayLeaderboard;}
bool Menu::getDisplayReplay() const {return displayReplay;}
bool Menu::getDisplayMenu() const {return displayMenu;}
void Menu::resetDisplayGame() {displayGame = false;}