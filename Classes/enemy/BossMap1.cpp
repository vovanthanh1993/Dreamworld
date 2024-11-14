#include "enemy/BossMap1.h"
#include "npc/NPC1.h"
#include "map/port.h"
#include "main/Effect.h"

BossMap1::BossMap1(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) :BaseCharacter(world, scene, position, _bodyToSpriteMap) {
};

void BossMap1::init() {
    spriteNode = SpriteBatchNode::create("Enemy/Bossmap1/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap1/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("0_boss_idle_0.png");
    sprite->setScale(Constants::BOSSMAP1_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP1);
    
    int* userData = new int(-1);
    sprite->setUserData(userData);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    //bodyDef.bullet = true;

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
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_PLAYER| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(Constants::SPEED_BOSS1 * Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(-Constants::BOSSMAP1_SCALE * Common::scaleSizeXY());
    (*_bodyToSpriteMap)[body] = sprite;
    createHealthBar();
    walk();
}
void BossMap1::idle() {

    if (sprite != nullptr) {
        b2Vec2 velocity(0, 0);
        body->SetLinearVelocity(velocity);
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("0_boss_idle_", 19, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void BossMap1::walk() {
    if (sprite != nullptr) {
        auto animateW = Animate::create(Common::createAnimation("0_boss_walk_", 19, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void BossMap1::die() {
    isALive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    Effect::soundLaughter();

    int round = Common::loadRound();
    if (round == 1) {
        Common::showBossText(scene, "Hmm. It really reminds me of the way you killed me back then...");
    }
    else if (round == 2) {
        Common::showBossText(scene, "We will meet again...");
    }
    else if (round == 3) {
        Common::showBossText(scene, "No. I lost again...");
    }
    else {
        Common::showBossText(scene, "You still have to kill me a thousand times...");
    }
   

    // Lặp qua tất cả các fixture của body
    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        // Lấy dữ liệu bộ lọc hiện tại
        b2Filter filter = fixture->GetFilterData();

        // Cập nhật categoryBits và maskBits
        filter.categoryBits = Constants::CATEGORY_ENEMY;
        filter.maskBits = Constants::CATEGORY_WALL;

        // Thiết lập lại dữ liệu bộ lọc
        fixture->SetFilterData(filter);
    }
    
    
    auto animate = Animate::create(Common::createAnimation("0_boss_die_", 19, 0.05));
    auto callback2 = [this]() {
        if (sprite != nullptr) {
            Gem gem;
            Vec2 pos = sprite->getPosition();
            gem.init(world, scene, pos, _bodyToSpriteMap, 10);
            sprite->removeFromParentAndCleanup(true);
            body->SetUserData(nullptr);
            (*_bodyToSpriteMap).erase(body);
            world->DestroyBody(body);
        }
        };
    auto callFunc2 = CallFunc::create(callback2);
    
    auto sequence = Sequence::create(animate, callFunc2, nullptr);
    sprite->runAction(sequence);
}

void BossMap1::hit() {
    if (isHit) return;
    // Run animation with a callback
    Fire* fire = new Fire();
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("0_boss_specialty_2_", 19, 0.015));
        Effect::soundMagicFire();

        auto callback = [this, fire]() {
            if (sprite != nullptr) {
                int check = 1;
                // check huong nhan vat
                if (sprite->getScaleX() < 0) {
                    check = -1;
                }

                fire->init(world, scene, Vec2(sprite->getPositionX() + check * 120 * Constants::BOSSMAP1_SCALE * Common::scaleSizeXY(), sprite->getPositionY()-80 * Common::scaleSizeY()), _bodyToSpriteMap);
                fire->getSprite()->setScaleX(check * fire->getSprite()->getScale());
                walk();
                b2Vec2 velocity(20 * check * Common::scaleSizeXY(), 0);
                fire->getBody()->SetLinearVelocity(velocity);
            }
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap1::phase2() {
    // Run animation with a callback
    if (isHit) return;
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("0_boss_specialty_", 19, 0.01));
        Effect::soundMagicFire();

        auto callback2 = [this](){
            int start = -50;
            for (int i = 1; i <= 20; i++) {
                FireRain* rain = new FireRain();
                if (sprite != nullptr) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }

                    rain->init(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY() + 800 * Common::scaleSizeY()), _bodyToSpriteMap);
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

void BossMap1::updateAttack(Player* player, float dt) {

    if (isHit) return;
    // Cập nhật thời gian đã trôi qua
    if (body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Constants::ATTACK_RANGE_BOSS_MAP1 * Common::scaleSizeXY()) {

                // Attack logic
                if(count++ % 5 != 0)
                  hit();
                else 
                    phase2();
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
            }
        }
    }
    
}
void BossMap1::setHealth(int h) {
    health = h;
};
int BossMap1::getHealth() {
    return health;
}

void BossMap1::updateHealth(int damage) {
        health -= damage;
        updateHealthBar(health);
        hurt();
        if (health <= 0) {
            healthBar->removeFromParentAndCleanup(true);
            healthBarBg->removeFromParentAndCleanup(true);
            die();
            
        } 
}

void BossMap1::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / maxHealth;
    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(healthRatio*Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY()); // Điều chỉnh chiều rộng thanh máu
}
void BossMap1::createHealthBar() {
    // Tạo nền thanh máu
    healthBarBg = Sprite::create("ui/health_bar_bg.png");
    healthBarBg->setPosition(position); // Đặt vị trí của nền thanh máu
    scene->addChild(healthBarBg);
    healthBarBg->setAnchorPoint(Vec2(0, 0));
    healthBarBg->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());

    // Tạo thanh máu
    healthBar = Sprite::create("ui/health_bar2.png");
    healthBar->setPosition(position); // Đặt cùng vị trí với nền
    scene->addChild(healthBar);
    healthBar->setAnchorPoint(Vec2(0,0));
    healthBar->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
}
void BossMap1::updateHealthBarPosition() {
    // Cập nhật sprite
    b2Vec2 position = body->GetPosition();
    int check = 1;
    if (sprite->getScaleX() < 0) {
        check = -1;
    }
    //healthBar->setAnchorPoint(Vec2(0.5,0.5));
    healthBar->setPosition(position.x * Constants::PIXELS_PER_METER - healthBar->getContentSize().width/4* Common::scaleSizeXY(), position.y* Constants::PIXELS_PER_METER + sprite->getContentSize().height/2 * Common::scaleSizeXY());
    healthBarBg->setPosition(position.x * Constants::PIXELS_PER_METER - healthBar->getContentSize().width/4 * Common::scaleSizeXY(), position.y * Constants::PIXELS_PER_METER + sprite->getContentSize().height/2 * Common::scaleSizeXY());
}

void BossMap1::hurt() {
    Effect::soundBoss1Hurt();
    Common::changeSpriteColor(sprite, isHit);

}