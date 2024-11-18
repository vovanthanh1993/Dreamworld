#include "player/skill/Stick.h"
#include "main/Effect.h"
void Stick::init(b2World* world, Scene* scene, Vec2 position, 
    unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    Effect::playerSlash();
    sprite = Sprite::create("Stick.png");
    sprite->setPosition(position);
    Common::scaleAll(sprite, 0.017);
    sprite->setTag(Constants::TAG_STICK);
    scene->addChild(sprite);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

   
    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_STICK;
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY| Constants::CATEGORY_WALL | Constants::CATEGORY_BLOCK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*_bodyToSpriteMap)[body] = sprite;
}
Stick::Stick(){}