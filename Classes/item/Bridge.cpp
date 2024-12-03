#include "Bridge.h"
Bridge::Bridge(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Bridge::init(Vec2 position) {
    scale = 0.9;

    auto sprite = Sprite::create("map/Bridge.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    if (type == 2) {
        sprite = Sprite::create("map/cave_bridge.png");
        sprite->setScale(scale * Common::scaleSizeXY());
    }
    sprite->setPosition(position);
    sprite->setTag(Constants::TAG_BRIDGE);
    scene->addChild(sprite);
    int* userData = new int(-1);
    sprite->setUserData(userData);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;


    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT | Constants::CATEGORY_STICK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*bodyToSpriteMap)[body] = sprite;
    b2Vec2 velocity(0, Constants::SPEED_BRIDGE * Common::scaleSizeXY());
    body->SetLinearVelocity(velocity);

    return true;
}