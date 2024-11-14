#include "NPC3.h"
#include "main/Effect.h"
#include "npc/DialogueBox.h"

NPC3::NPC3(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    spriteNode = SpriteBatchNode::create("NPC/NPC3/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("NPC/NPC3/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Mater_Idle_0.png");
    sprite->setScale(Constants::NPC3_SCALE* Common::scaleSizeXY());
    
    int* userData = new int(-1);
    sprite->setUserData(userData);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 0);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    //bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*_bodyToSpriteMap)[body] = sprite;
    idle();
    
}
void NPC3::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Mater_Idle_", 17, 0.1));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
    
}


Sprite* NPC3::getSprite() {
    return sprite;
}

b2Body* NPC3::getBody() {
    return body;
}

SpriteBatchNode* NPC3::getSpriteNode() {
    return spriteNode;
};
void NPC3::startConversation(Scene* scene) {
    displayMessage("Xin chào! Bạn có cần giúp đỡ không?", scene);
}
void NPC3::displayMessage(const std::string& message, Scene* scene) {
    // Khởi tạo NPC
    _nameLabel = Label::createWithTTF("Shop", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY());
    _nameLabel->setPosition(sprite->getPositionX(), sprite->getPositionY() + sprite->getContentSize().height / 2 * sprite->getScale() + 15 * Common::scaleSizeXY()); // Vị trí có thể thay đổi
    scene->addChild(_nameLabel);
}