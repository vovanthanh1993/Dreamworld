#include "Copy.h"
#include "main/Effect.h"
Sprite* Copy::getSprite() {
    return sprite;
}

b2Body* Copy::getBody() {
    return body;
}
void Copy::init(b2World* world, Scene* scene, Vec2 position,
    unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    Effect::playerSlash();
    sprite = Sprite::create("player/skill/cloud.png");
    sprite->setPosition(position);
    sprite->setScale(0.2 * Common::scaleSprite());
   // sprite->setAnchorPoint(Vec2(0,0));
    sprite->setTag(Common::TAG_COPY);
    scene->addChild(sprite);
    
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
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_STICK;
    fixtureDef.filter.maskBits = Common::CATEGORY_ENEMY | Common::CATEGORY_BLOCK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*_bodyToSpriteMap)[body] = sprite;

}