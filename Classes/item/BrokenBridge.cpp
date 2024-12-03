#include "BrokenBridge.h"
BrokenBridge::BrokenBridge(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool BrokenBridge::init(Vec2 position) {
    scale = 0.9;

    auto sprite = Sprite::create("map/BridgeBreak.png");
    sprite->setPosition(position);
    sprite->setScale(Constants::BRIDGE_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BRIDGE_BREAK);
    scene->addChild(sprite);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(position.x / Constants::PIXELS_PER_METER, position.y / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;


    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width - 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height - 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1000000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
    fixtureDef.filter.maskBits = Constants::TAG_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*bodyToSpriteMap)[body] = sprite;

    return true;
}