//
// Created by TeamsInsane on 24. 02. 2022.
//

#ifndef ANIMALGAME_OBJECTFACTORY_H
#define ANIMALGAME_OBJECTFACTORY_H

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "../Objects/GameObject.h"

class ObjectFactory {
private:
    ObjectFactory();
    static ObjectFactory *instance;
    std::map<std::string, std::function<GameObject*(Properties* properties)>> typeRegistry;
public:
    GameObject *createObject(std::string type, Properties *properties);
    void registerType(std::string className, std::function<GameObject*(Properties *properties)> type);
    static ObjectFactory *getInstance();
};

template<class T>
class Registrar{
public:
    Registrar(std::string className) {
        ObjectFactory::getInstance()->registerType(className, [](Properties *properties) -> GameObject * {
            return new T(properties);
        });
    }
};


#endif //ANIMALGAME_OBJECTFACTORY_H
