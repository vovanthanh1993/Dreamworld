#include "Eagle.h"
#include "main/Effect.h"
Eagle::Eagle(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {}

bool Eagle::init(Vec2 position) {
    scale = 0.3;
    isActive = true;

    auto spriteNode = SpriteBatchNode::create("player/skill/bear/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/skill/bear/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Animal_02__FLY_0.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    spriteNode->addChild(sprite);
    sprite->setUserData(this);

    sprite->setTag(Constants::TAG_EAGLE);
    scene->addChild(spriteNode);
    auto animateW = Animate::create(Common::createAnimation("Animal_02__FLY_", 9, 0.06));
    animateW->retain();
    sprite->runAction(RepeatForever::create(animateW));
    
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
    fixtureDef.density = 0.001f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_STICK;
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY | Constants::CATEGORY_BLOCK;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    (*bodyToSpriteMap)[body] = sprite;
    return true;
}