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
    bool repeated;
    float scrollRatio;
    std::string textureID;
    float scaleX, scaleY;
    int offsetX, offsetY;
    int imgWidth, imgHeight;
    SDL_RendererFlip flip;
public:
    ImgLayer(std::string textureID, int x, int y, int width, int height, float scrollRatio = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);
    void setScrollRatio(float scrollRatio) {this->scrollRatio = scrollRatio;}
    void setOffset(int x, int y) {offsetX = x; offsetY = y;}
    void setRepeated(bool repeated) {this->repeated = repeated;}
    void queryImage(std::string textureID);

    virtual void render();
    virtual void update();
};


#endif //ANIMALGAME_IMGLAYER_H
