#include "Stone.h"
Stone::Stone(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Stone::init(Vec2 position) {
    scale = 0.2;

    auto sprite = Sprite::create("map/stone.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOX);
    scene->addChild(sprite, 0);
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
    fixtureDef.density = 500.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_STONE;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_LIMIT | Constants::CATEGORY_WALL;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;

    return true;
}