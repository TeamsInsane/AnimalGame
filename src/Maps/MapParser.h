//
// Created by TeamsInsane on 23. 02. 2022.
//

#ifndef ANIMALGAME_MAPPARSER_H
#define ANIMALGAME_MAPPARSER_H

#include "../Vendor/tinyxml.h"
#include "GameMap.h"
#include "TileLayer.h"

#include <map>
#include <string>
#include <SDL.h>

class MapParser {
private:
    static MapParser *instance;
    std::map<std::string, GameMap*> mapDict;
    MapParser();
    bool parse(std::string id, std::string source);
    TileSet parseTileSet(TiXmlElement *xmlTileSet);
    TileLayer* parseTileLayer(TiXmlElement *xmlLayer, TileSetList tileSets, int tileSize, int rowCount, int colCount);
public:
    static MapParser *getInstance();
    bool load(std::string source);
    void clean();
    GameMap *getMaps(std::string id) {return mapDict[id];}
};


#endif //ANIMALGAME_MAPPARSER_H
