#include "Skull.h"

void Skull::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    /*auto spriteNode = SpriteBatchNode::create("Enemy/Bossmap1/fire/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap1/fire/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("9.png");*/
    sprite = Sprite::create("Enemy/Bossmap2/skull/skull.png");
    sprite->setPosition(position);
    sprite->setScale(Constants::SKULL_SCALE * Common::scaleSizeXY());
    //sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setTag(Constants::TAG_SKULL);

   /* auto animateW = Animate::create(Constants::createAnimation("", 27, 0.1));
    animateW->retain();
    sprite->runAction(RepeatForever::create(animateW));*/
    
    scene->addChild(sprite);

    
    
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
    fixtureDef.restitution = 1.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ARROW;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_WALL | Constants::CATEGORY_BLOCK;;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*_bodyToSpriteMap)[body] = sprite;

}
Skull::Skull(){}