//
// Created by TeamsInsane on 3. 03. 2022.
//

#ifndef ANIMALGAME_IMGLAYER_H
#define ANIMALGAME_IMGLAYER_H

#include <string>
#include "Layer.h"
#include <SDL.h>

class ImgLayer : public Layer {
private:
    float scrollRatio;
    std::string textureID;
    float scaleX, scaleY;
    int offsetX, offsetY;
    int imgWidth, imgHeight;
public:
    ImgLayer(std::string textureID, int x, int y, int width, int height, float scrollRatio = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);

    virtual void render();
    virtual void update();
};


#endif //ANIMALGAME_IMGLAYER_H
