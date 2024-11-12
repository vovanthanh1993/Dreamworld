#include "Heart.h"
#include "player/Player.h"
Sprite* Heart::getSprite() {
    return sprite;
}

b2Body* Heart::getBody() {
    return body;
}
void Heart::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    sprite = Sprite::create("Item/gourd/heart.png");
    sprite->setPosition(position);
    sprite->setScale(0.08 * Common::scaleSprite());
    sprite->setTag(Common::TAG_HEART);
    scene->addChild(sprite);
    auto rotateAction = RotateBy::create(2.0f, 360); // Thời gian 2 giây, góc 360 độ
    sprite->runAction(RepeatForever::create(rotateAction)); // Lặp lại mãi mãi

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER, sprite->getPositionY() / Common::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.00001f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_ITEM;
    fixtureDef.filter.maskBits = Common::CATEGORY_WALL | Common::CATEGORY_PLAYER;

    // Gán fixture cho body
    body->SetGravityScale(0.0f);
    body->CreateFixture(&fixtureDef);
    (*_bodyToSpriteMap)[body] = sprite;
    Common::zoomAction(sprite);
}