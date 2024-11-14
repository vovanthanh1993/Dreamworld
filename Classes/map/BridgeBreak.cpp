#include "map/BridgeBreak.h"
Sprite* BridgeBreak::getSprite() {
    return sprite;
}

b2Body* BridgeBreak::getBody() {
    return body;
}
void BridgeBreak::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    sprite = Sprite::create("map/BridgeBreak.png");
    sprite->setPosition(position);
    sprite->setScale(Constants::BRIDGE_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BRIDGE_BREAK);
    scene->addChild(sprite);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;
    
   
    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width- 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height- 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1000000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
    fixtureDef.filter.maskBits = Constants::TAG_PLAYER | Constants::CATEGORY_ARROW| Constants::CATEGORY_LIMIT;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*_bodyToSpriteMap)[body] = sprite;
}