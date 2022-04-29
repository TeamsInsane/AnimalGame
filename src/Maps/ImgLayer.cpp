//
// Created by TeamsInsane on 3. 03. 2022.
//

#include "ImgLayer.h"
#include "../Graphics/TextureManager.h"

ImgLayer::ImgLayer(std::string textureID, int x, int y, int width, int height, float scrollRatio, float scaleX, float scaleY) {
    this->textureID = textureID;
    this->offsetX = x;
    this->offsetY = y;
    this->scrollRatio = scrollRatio;
    this-> scaleX = scaleX;
    this->scaleY = scaleY;
    this->imgWidth = width;
    this->imgHeight = height;
}

void ImgLayer::render() {
    TextureManager::getInstance()->drawTexture(textureID, offsetX, offsetY, imgWidth, imgHeight, scaleX, scaleY, scrollRatio);
}

void ImgLayer::update() {

}
