//
// Created by TeamsInsane on 24. 02. 2022.
//

#include "ObjectFactory.h"

ObjectFactory *ObjectFactory::instance = nullptr;

ObjectFactory::ObjectFactory() {

}

ObjectFactory *ObjectFactory::getInstance() {
    if (instance == nullptr) instance = new ObjectFactory();
    return instance;
}

GameObject *ObjectFactory::createObject(std::string type, Properties *properties) {
    GameObject *gameObject = nullptr;

    auto it = typeRegistry.find(type);

    if (it != typeRegistry.end()) gameObject = it->second(properties);

    return gameObject;
}

void ObjectFactory::registerType(std::string className, std::function<GameObject *(Properties *)> type) {
    typeRegistry[className] = type;
}
