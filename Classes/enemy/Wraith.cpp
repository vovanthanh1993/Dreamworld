#include "Wraith.h"

Wraith::Wraith(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};

bool Wraith::init() {
    spriteNode = SpriteBatchNode::create("Enemy/Wraith/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Wraith/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wraith_3_Idle_0.png");
    sprite->setScale(Constants::WRAITH_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_ELEMENT);

    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode);

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
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_LIMIT | Constants::CATEGORY_SLASH;//Constants::CATEGORY_STICK| ;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    sprite->setScaleX(-Constants::WRAITH_SCALE * Common::scaleSizeXY());
    idle();

    (*bodyToSpriteMap)[body] = sprite;

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "wraith");
    scene->addChild(this);
    return true;
}
void Wraith::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Wraith_3_Idle_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Wraith::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Wraith_3_Moving Forward_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}


void Wraith::hit() {
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Wraith_3_Casting Spells_", 17, 0.01));

        auto callback2 = [this]() {
            int start = -40;
            for (int i = 1; i <= 9; i++) {
                
                if (sprite != nullptr) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }
                    Rain* rain = new Rain(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY()), bodyToSpriteMap);
                    rain->init();
                    rain->getSprite()->setScaleX(check * rain->getSprite()->getScale());
                    b2Vec2 velocity(start * Common::scaleSizeXY(), -20 * Common::scaleSizeXY());
                    rain->getBody()->SetLinearVelocity(velocity);
                    start += 10;
                }
            }

            };

        // auto callFunc1 = CallFunc::create(callback1);
        auto callFunc2 = CallFunc::create(callback2);

        auto sequence = Sequence::create(animate, callFunc2, nullptr);
        sprite->runAction(sequence);
    }
}
void Wraith::update(float dt) {
    // Cập nhật thời gian đã trôi qua
    if (body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Constants::ATTACK_RANGE_WRAITH * Common::scaleSizeXY()) {

                // Attack logic
                hit();
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
            }
        }
    }
    
}

//void Wraith::die() {
//    b2Vec2 velocity(0, 0);
//    body->SetLinearVelocity(velocity);
//    sprite->stopAllActions();
//    auto animate = Animate::create(Common::createAnimation("Wraith_3_Dying_", 14, 0.05));
//    Effect::enemyDie();
//    // Lặp qua tất cả các fixture của body
//    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
//        // Lấy dữ liệu bộ lọc hiện tại
//        b2Filter filter = fixture->GetFilterData();
//
//        // Cập nhật categoryBits và maskBits
//        filter.categoryBits = Constants::CATEGORY_ENEMY;
//        filter.maskBits = Constants::CATEGORY_WALL;
//
//        // Thiết lập lại dữ liệu bộ lọc
//        fixture->SetFilterData(filter);
//    }
//
//
//    auto callback = [this]() {
//        if (sprite != nullptr) {
//            sprite->removeFromParentAndCleanup(true);
//            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, 1);
//            body->SetUserData(nullptr);
//            world->DestroyBody(body);
//        }
//        };
//    auto callFunc = CallFunc::create(callback);
//    auto sequence = Sequence::create(animate, callFunc, nullptr);
//    sprite->runAction(sequence);
//}
