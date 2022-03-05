//
// Created by Uporabnik on 4. 03. 2022.
//

#ifndef ANIMALGAME_PLAY_H
#define ANIMALGAME_PLAY_H


#include <SDL.h>
#include "../Maps/GameMap.h"
#include "../Core/Engine.h"
#include "../Characters/Warrior.h"
#include "../Characters/Animals.h"

class Play {
private:
    static Play* instance;
public:
    static Play *getInstance();
    void mainGame(GameMap *&levelMap, std::vector<ImgLayer*> &parallaxBg, Warrior *&player, std::vector<GameObject*> &gameObject, Animals *&animal);

    Animals *renderAnimal();
};


#endif //ANIMALGAME_PLAY_H
