//
// Created by TeamsInsane on 17. 02. 2022.
//

#ifndef ANIMALGAME_TEXTUREMANAGER_H
#define ANIMALGAME_TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>

class TextureManager{
private:
    static TextureManager* instance;
    std::map<std::string, SDL_Texture*> textureMap;
public:
    static TextureManager *getInstance();

    bool parseTextures(const std::string& source);
    bool loadTexture(const std::string& id, const std::string& filename);
    void dropTexture(const std::string& id);

    void cleanTexture();

    void drawTexture(const std::string& id, int x, int y, int width, int height, float scaleX = 1, float scaleY = 1, float scrollRatio = 0, SDL_RendererFlip flip= SDL_FLIP_NONE);
    void drawFrame(const std::string& id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawTile(const std::string& tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif //ANIMALGAME_TEXTUREMANAGER_H
