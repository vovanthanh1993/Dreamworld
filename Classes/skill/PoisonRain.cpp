#include "PoisonRain.h"
PoisonRain::PoisonRain(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) : BaseNode(world, scene, bodyToSpriteMap) {};

bool PoisonRain::init(Vec2 position) {
    scale = 0.5;
    isActive = true;

    sprite = Sprite::create("Enemy/Bossmap1/rain/rain.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_RAIN);
    scene->addChild(sprite);
    sprite->setUserData(this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;
    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((((sprite->getContentSize().width) / 2-20) * sprite->getScale()) / Constants::PIXELS_PER_METER,
        (((sprite->getContentSize().height) / 2-20) * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ARROW;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_WALL | Constants::CATEGORY_BLOCK;;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*bodyToSpriteMap)[body] = sprite;

    return true;
}