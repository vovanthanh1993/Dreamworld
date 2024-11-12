#include "BaseItem.h"
#include <iostream>
#include <fstream>

BaseItem::BaseItem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map) {
    // Store the provided scene reference
    this->scene = scene;

    // Store the provided world reference
    this->world = world;

    // Store the reference to the body-to-sprite map
    this->_bodyToSpriteMap = _bodyToSpriteMap;

    // Initialize the character's position
    this->map = map;
}

Sprite* BaseItem::getSprite() {
    return sprite;
}

b2Body* BaseItem::getBody() {
    return body;
}

SpriteBatchNode* BaseItem::getSpriteNode() {
    return spriteNode;
};
BaseItem::BaseItem() {};