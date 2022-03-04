cmake_minimum_required(VERSION 3.17)
project(AnimalGame)

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_IMAGE REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
find_package(SDL2_MIXER REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIR})
find_package(SDL2_TTF REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_executable(AnimalGame src/main.cpp src/Core/Engine.cpp src/Core/Engine.h src/Graphics/TextureManager.cpp src/Graphics/TextureManager.h src/Physics/Vector2D.h src/Physics/Transform.h src/Objects/IObject.h src/Objects/GameObject.h src/Characters/Character.h src/Characters/Warrior.cpp src/Characters/Warrior.h src/Animations/Animation.h src/Physics/RigidBody.h src/Inputs/Input.cpp src/Inputs/Input.h src/Timers/Timer.cpp src/Timers/Timer.h src/Vendor/tinystr.cpp src/Vendor/tinystr.h src/Vendor/tinyxml.cpp src/Vendor/tinyxml.h src/Vendor/tinyxmlerror.cpp src/Vendor/tinyxmlparser.cpp src/Maps/Layer.h src/Maps/GameMap.h src/Maps/TileLayer.cpp src/Maps/TileLayer.h src/Maps/MapParser.cpp src/Maps/MapParser.h src/Cameras/Camera.cpp src/Cameras/Camera.h src/Physics/Point.h src/Collisions/CollisionHandler.cpp src/Collisions/CollisionHandler.h src/Physics/Collider.h src/Animations/SeqAnimation.cpp src/Animations/SeqAnimation.h src/Animations/SpriteAnimation.cpp src/Animations/SpriteAnimation.h src/Characters/Enemy.cpp src/Characters/Enemy.h src/Factory/ObjectFactory.cpp src/Factory/ObjectFactory.h src/Sound/SoundManager.cpp src/Sound/SoundManager.h src/Maps/ImgLayer.cpp src/Maps/ImgLayer.h src/Characters/Animals.cpp src/Characters/Animals.h src/ScreenText/TextDisplays.cpp src/ScreenText/TextDisplays.h src/Game/Play.cpp src/Game/Play.h src/Game/Menu.cpp src/Game/Menu.h src/Game/Text.cpp src/Game/Text.h)

target_link_libraries(AnimalGame ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})