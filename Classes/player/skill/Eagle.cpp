#include "Eagle.h"
#include "main/Effect.h"
void Eagle::init(b2World* world, Scene* scene, Vec2 position,
    unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {

    auto spriteNode = SpriteBatchNode::create("player/skill/bear/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/skill/bear/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Animal_02__FLY_0.png");
    sprite->setPosition(position);
    sprite->setScale(0.5 * Common::scaleSprite());
    spriteNode->addChild(sprite);

    sprite->setTag(Common::TAG_CLOUD);
    scene->addChild(spriteNode);
    auto animateW = Animate::create(Common::createAnimation("Animal_02__FLY_", 9, 0.06));
    animateW->retain();
    sprite->runAction(RepeatForever::create(animateW));
    
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
    fixtureDef.density = 100.f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_STICK;
    fixtureDef.filter.maskBits = Common::CATEGORY_ENEMY | Common::CATEGORY_BLOCK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*_bodyToSpriteMap)[body] = sprite;
}