#include "BaseCharacter.h"
#include <iostream>
#include <fstream>

BaseCharacter::BaseCharacter(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    // Store the provided scene reference
    this->scene = scene;

    // Store the provided world reference
    this->world = world;

    // Store the reference to the body-to-sprite map
    this->_bodyToSpriteMap = _bodyToSpriteMap;

    // Initialize the character's position
    this->position = position;
}

Sprite* BaseCharacter::getSprite() {
    return sprite;
}

b2Body* BaseCharacter::getBody() {
    return body;
}

SpriteBatchNode* BaseCharacter::getSpriteNode() {
    return spriteNode;
};