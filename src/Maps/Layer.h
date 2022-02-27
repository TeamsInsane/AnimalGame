//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_LAYER_H
#define ANIMALGAME_LAYER_H


class Layer {
public:
    virtual void render()=0;
    virtual void update()=0;
};


#endif //ANIMALGAME_LAYER_H
