#include "player/PlayerFly.h"
#include "main/Effect.h"
#include "player/skill/Stick.h"
#include "player/skill/Eagle.h"
#include "scene/GameOver.h"
#include <iostream>
#include <fstream>

PlayerFly::PlayerFly(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};
bool PlayerFly::init(bool isNew) {

    spriteNode = SpriteBatchNode::create("player/fly/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/fly/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wukong-FlyOnStick_0.png");
    sprite->setScale(Constants::PLAYER_SCALE* Common::scaleSizeXY());
    sprite->setPosition(position);
    sprite->setTag(Constants::TAG_PLAYER);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 1);

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
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_PLAYER;
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY|Constants::CATEGORY_LIMIT_MAP|Constants::CATEGORY_SLASH_ENEMY | Constants::CATEGORY_WALL| Constants::CATEGORY_ITEM| Constants::CATEGORY_CHEST| Constants::CATEGORY_ARROW | Constants::CATEGORY_STONE| Constants::CATEGORY_GEM; //  Không va chạm với CATEGORY_B
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    body->SetGravityScale(0.0f);

    // load from file
    //scene->addChild(uiNode, 10);
    idle();

    /*initKeyEvent();
    initMouseEvent();*/

    //// Lên lịch gọi update mỗi frame
    //this->schedule([this](float dt) { this->update(dt); }, "PlayerFly");
    //scene->addChild(this);

    
    return true;
}
void PlayerFly::idle() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-FlyOnStick_", 9, 0.04));
    animate->retain();
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(2); // Gán tag để quản lý hoạt ảnh
    sprite->runAction(repeatAnimate);
}

void PlayerFly::walk() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Walk_", 17, 0.02));
    animate->retain();
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(1);
    sprite->runAction(repeatAnimate);
}

void PlayerFly::update(float dt) {

    //if (scene->getChildByName("shop") == nullptr) {
    //    isEnable = true;
    //}
    //if (!isEnable||!isAlive) return;

    //if (keys[EventKeyboard::KeyCode::KEY_A]) {
    //    direction = -1;
    //    body->SetLinearVelocity(b2Vec2(-Constants::SPEED_PlayerFly * Common::scaleSizeXY(), body->GetLinearVelocity().y)); // Di chuyển sang trái
    //    sprite->setScaleX(-Constants::PlayerFly_SCALE * Common::scaleSizeXY());
    //    if (!sprite->getActionByTag(4) && !sprite->getActionByTag(1)) { // Kiểm tra nếu hoạt ảnh chưa chạy
    //        walk();
    //    }
    //}
    //else if (keys[EventKeyboard::KeyCode::KEY_D]) {
    //    direction = 1;
    //    body->SetLinearVelocity(b2Vec2(Constants::SPEED_PlayerFly * Common::scaleSizeXY(), body->GetLinearVelocity().y)); // Di chuyển sang phải
    //    sprite->setScaleX(Constants::PlayerFly_SCALE * Common::scaleSizeXY());
    //    if (!sprite->getActionByTag(4) && !sprite->getActionByTag(1)) { // Kiểm tra nếu hoạt ảnh chưa chạy
    //        walk();
    //    }
    //}
    //else {
    //    // Dừng lại nếu không nhấn phím nào
    //    body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
    //    if (!sprite->getActionByTag(4) && !sprite->getActionByTag(2)) { // Kiểm tra nếu hoạt ảnh chưa chạy
    //        idle();
    //    }
    //} 

    
}