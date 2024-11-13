#include "Bridge.h"
Sprite* Bridge::getSprite() {
    return sprite;
}

b2Body* Bridge::getBody() {
    return body;
}
void Bridge::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    sprite = Sprite::create("map/Bridge.png");
    sprite->setPosition(position);
    sprite->setScale(Common::BRIDGE_SCALE * Common::scaleSprite());
    sprite->setTag(Common::TAG_BRIDGE);
    scene->addChild(sprite);
    int* userData = new int(-1);
    sprite->setUserData(userData);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER, sprite->getPositionY() / Common::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;
    
   
    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width- 20) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
        ((sprite->getContentSize().height-20) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_WALL;
    fixtureDef.filter.maskBits = Common::CATEGORY_PLAYER | Common::CATEGORY_ARROW| Common::CATEGORY_LIMIT| Common::CATEGORY_STICK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*_bodyToSpriteMap)[body] = sprite;
    b2Vec2 velocity(0, Common::SPEED_BRIDGE * Common::scaleSprite());
    body->SetLinearVelocity(velocity);
}