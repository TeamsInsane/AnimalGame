//
// Created by Uporabnik on 23. 02. 2022.
//

#include "SeqAnimation.h"
#include "../Graphics/TextureManager.h"
#include "../Vendor/tinyxml.h"

SeqAnimation::SeqAnimation(bool repeat) : Animation(repeat) {}

void SeqAnimation::update(float dt) {

    if (repeat || !isEnded){
        isEnded = false;
        currentFrame = (SDL_GetTicks() / currentSeq.sequenceSpeed) % currentSeq.frameCount;
    }

    if (!repeat && currentFrame == (currentSeq.frameCount - 1)){
        isEnded = true;
        currentFrame = (currentSeq.frameCount - 1);
    }
}

void SeqAnimation::parse(std::string source) {
    TiXmlDocument xmlDocument;
    xmlDocument.LoadFile(source);
    if (xmlDocument.Error()) SDL_Log("Failed to load animation file: %s", source.c_str());

    TiXmlElement *root = xmlDocument.RootElement();
    for(TiXmlElement *element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()){
        if (element->Value() == std::string("sequence")){
            Sequence sequence;
            std::string seqID = element->Attribute("id");
            element->Attribute("speed", &sequence.sequenceSpeed);
            element->Attribute("width", &sequence.width);
            element->Attribute("height", &sequence.height);
            element->Attribute("frameCount", &sequence.frameCount);
            for(TiXmlElement *frame = element->FirstChildElement(); frame != nullptr; frame = frame->NextSiblingElement()){
                sequence.textureIDs.push_back(frame->Attribute("textureID"));
                seqMap[seqID] = sequence;
            }
        }
    }
}

void SeqAnimation::setCurrentSeq(std::string seqID) {
    if (seqMap.count(seqID) > 0) currentSeq = seqMap[seqID];
    else SDL_Log("The given Sequence animation is not matching: %s", seqID.c_str());
}

void SeqAnimation::drawFrame(float x, float y, float xScale, float yScale, SDL_RendererFlip flip) {
    TextureManager::getInstance()->drawTexture(currentSeq.textureIDs[currentFrame], x, y, currentSeq.width, currentSeq.height, xScale, yScale,  1, flip);
}
