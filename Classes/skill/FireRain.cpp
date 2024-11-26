#include "FireRain.h"
FireRain::FireRain(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {};

bool FireRain::init() {
    auto spriteNode = SpriteBatchNode::create("Enemy/Bossmap1/firerain/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap1/firerain/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("fire_rain_0.png");
    //sprite = Sprite::create("Enemy/Bossmap1/rain/rain.png");
    sprite->setPosition(position);
    sprite->setScale(Constants::FIRE_RAIN_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_FIRE_RAIN);

    auto animateW = Animate::create(Common::createAnimation("fire_rain_", 8, 0.1));
    animateW->retain();
    sprite->runAction(RepeatForever::create(animateW));
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode);

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