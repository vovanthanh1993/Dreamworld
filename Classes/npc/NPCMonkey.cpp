#include "NPCMonkey.h"
NPCMonkey::NPCMonkey(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};

bool NPCMonkey::init() {
    spriteNode = SpriteBatchNode::create("npc/NPCMonkey/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("npc/NPCMonkey/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("MonkeyNPCRun_0.png");
    Common::scaleAll(sprite, 0.03);
    sprite->setTag(Constants::TAG_NPC);
    
    int* userData = new int(-1);
    sprite->setUserData(userData);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 0);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_NPC;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_LIMIT;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(Constants::SPEED_ENEMY* Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(-sprite->getScale());
    (*bodyToSpriteMap)[body] = sprite;
    walk();

    return true;
}

void NPCMonkey::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("MonkeyNPCRun_", 13, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}
