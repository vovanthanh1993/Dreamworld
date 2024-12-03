#include "Chest.h"
Chest::Chest(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Chest::init(Vec2 position) {

    scale = 0.15;

    auto sprite = Sprite::create("Item/chest/closeChest.png");
    sprite->setPosition(position);
    sprite->setScale(0.15 * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_CHEST);
    scene->addChild(sprite);
    sprite->setUserData(this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(position.x / Constants::PIXELS_PER_METER, position.y / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_CHEST;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_SLASH;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    return true;
}