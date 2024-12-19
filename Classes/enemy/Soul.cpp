#include "Soul.h"
Soul::Soul(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {};
bool Soul::init(Vec2 position) {
    scale = 0.3;
    isActive = true;
    health = 20;

    sprite = Sprite::create("Item/soul/soul2.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_SOUL);
    scene->addChild(sprite);
    sprite->setUserData(this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(position.x / Constants::PIXELS_PER_METER, position.y / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ITEM;
    fixtureDef.filter.maskBits = Constants::CATEGORY_SLASH;

    // Gán fixture cho body
    body->SetGravityScale(0.0f);
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
   Common::zoomAction(sprite);
    //sprite->setColor(Color3B(0, 0, 0));
    return true;
}

void Soul::getDamage(int damage) {
    health -= damage;
    hurt();
    
    
    if (health <= 0) {
        /*healthBar->removeFromParentAndCleanup(true);
        healthBarBg->removeFromParentAndCleanup(true);*/
        die();

    }
}

void Soul::hurt() {
    Common::changeSpriteColor(sprite, isHit);
}

void Soul:: die() {
    isAlive = false;
    /*auto animate = Animate::create(Common::createAnimation("0_boss_die_", 19, 0.05));*/
    Effect::smoke(world, scene, sprite->getPosition());
    auto callback2 = [this]() {
        if (!isAlive) {
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, 10);
            Common::spawnCharm(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 2));
            BaseNode::destroyNode();
        }
        };
    auto callFunc2 = CallFunc::create(callback2);

    auto sequence = Sequence::create(callFunc2, nullptr);
    sprite->runAction(sequence);
}