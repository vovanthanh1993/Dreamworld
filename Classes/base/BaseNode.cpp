#include "BaseNode.h"

BaseNode::BaseNode(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) {

    this->scene = scene;
    this->world = world;
    this->position = position;
    this->bodyToSpriteMap = bodyToSpriteMap;
    
}

BaseNode::BaseNode(b2World* world, Scene* scene, Vec2 position) {

    this->scene = scene;
    this->world = world;
    this->position = position;
}

BaseNode::BaseNode(b2World* world, Scene* scene) {
    this->scene = scene;
    this->world = world;
}

BaseNode::BaseNode(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) {
    this->scene = scene;
    this->world = world;
    this->bodyToSpriteMap = bodyToSpriteMap;
}

Sprite* BaseNode::getSprite() {
    return sprite;
}

b2Body* BaseNode::getBody() {
    return body;
}

SpriteBatchNode* BaseNode::getSpriteNode() {
    return spriteNode;
};

void BaseNode::destroyNode() {
    bodyToSpriteMap->erase(body);

    // Hủy body Box2D nếu nó tồn tại
    if (body)
    {
        world->DestroyBody(body);
        body = nullptr; // Đảm bảo body không còn trỏ tới bất kỳ bộ nhớ nào
    }

    // Xóa sprite nếu nó tồn tại
    if (sprite)
    {
        sprite->removeFromParentAndCleanup(true); // Xóa node Cocos2d
        sprite = nullptr; // Đảm bảo sprite không còn trỏ tới bất kỳ bộ nhớ nào
    }
    
}


// Phương thức Get cho isActive
bool BaseNode::getIsActive() {
    return isActive;
}

// Phương thức Set cho isActive
void BaseNode::setIsActive(bool active) {
    isActive = active;
}

// Phương thức Get cho scale
float BaseNode::getScale() {
    return scale;
}

// Phương thức Set cho scale
void BaseNode::setScale(float newScale) {
    scale = newScale;
}

void BaseNode::reset() {
    isActive = false;
}