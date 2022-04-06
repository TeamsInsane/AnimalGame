//
// Created by TeamsInsane on 4. 04. 2022.
//

#include <fstream>
#include <SDL_log.h>
#include <iostream>
#include <SDL_image.h>
#include "Leaderboard.h"

Leaderboard *Leaderboard::instance = nullptr;

Leaderboard *Leaderboard::getInstance() {
    if (instance == nullptr) instance = new Leaderboard;
    return instance;
}

void Leaderboard::updateMap(){
    map.clear();
    std::ifstream data("Leaderboard.txt");
    if (!data.is_open()){
        SDL_Log("File was not found!");
        return;
    }

    std::string name;
    int score;
    while (data >> name >> score){
        SDL_Log("From file name: %s and score %i: ", name.c_str(), score);
        map[name] = score;
    }

    for(auto it = map.begin(); it != map.end(); it++){
        SDL_Log("Name -> %s and score -> %i", it->first.c_str(), it->second);
    }

    data.close();
}

void Leaderboard::addToFile(std::string name, int score){
    bool found = false;
    updateMap();
    for(auto it = map.begin(); it != map.end(); it++)
        if (it->first == name && it->second > score) found = true;

    if (!found) map[name] = score;

    std::ofstream data("Leaderboard.txt");
    while (!map.empty()) {
        std::string highName = map.begin()->first;
        int highScore = map.begin()->second;
        for (auto it = map.begin(); it != map.end(); it++) {
            if (highScore < it->second){
                highScore = it->second;
                highName = it->first;
            }
        }
        map.erase(highName);
        data << highName << " " << highScore << std::endl;
    }

    data.close();
}

void Leaderboard::init(SDL_Renderer *renderer){
    this->renderer = renderer;
    SDL_Surface *surface = IMG_Load("../assets/images/PepaPig.png");
    background = SDL_CreateTextureFromSurface(renderer, surface);
    bgRect = {0, 0, 1920, 1080};
    SDL_FreeSurface(surface);
    char tempText[51];
    strcpy(tempText, "Lestvica najboljsih igralcev:");
    int y = 200;
    leaderboard[0].initCenter(renderer, 40, 30, tempText);
    leaderboardCount = 1;

    std::ifstream data("Leaderboard.txt");
    if (!data.is_open()){
        SDL_Log("Couldn't open the file!");
        return;
    }

    std::string name;
    int score;
    leaderboardCount = 1;
    while (data >> name >> score){
        strcpy(tempText, std::to_string(leaderboardCount).c_str());
        strcat(tempText, ". ");
        strcat(tempText, name.c_str());
        strcat(tempText, " - ");
        strcat(tempText, std::to_string(score).c_str());
        leaderboard[leaderboardCount].initCenter(renderer, y, 30, tempText);
        leaderboardCount++;
        y+=100;
        if (leaderboardCount >= 6) break;
    }

    SDL_Log("Leaderboard count: %i", leaderboardCount);

    data.close();
}

void Leaderboard::draw(){
    SDL_RenderCopyEx(renderer, background, nullptr, &bgRect, 0, nullptr, SDL_FLIP_NONE);
    for(int i = 0; i < leaderboardCount; i++) leaderboard[i].draw();
}