#include "BossMap3.h"

BossMap3::BossMap3(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};

bool BossMap3::init() {
    spriteNode = SpriteBatchNode::create("Enemy/BossMap3/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/BossMap3/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("troll_idle_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP3);
    
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
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(direction*speed * Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(direction* scale * Common::scaleSizeXY());
    (*bodyToSpriteMap)[body] = sprite;

    createHealthBar();
    walk();

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "BossMap3");
    scene->addChild(this);
    return true;
}
void BossMap3::idle() {
    if (sprite != nullptr) {
        b2Vec2 velocity(0, 0);
        body->SetLinearVelocity(velocity);
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("troll_idle Blinking_", 11, 0.1));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void BossMap3::hurt() {

    Effect::soundBoss1Hurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("troll_hurt_", 11, 0.1));
    auto callback = [this]() {
        walk();
    };

    // auto callFunc1 = CallFunc::create(callback1);
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
    Common::changeSpriteColor(sprite, isHit);
}

void BossMap3::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("troll_walking_", 17, 0.1));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }
}

void BossMap3::die() {
    isALive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();

    int round = Common::loadRound();
    if (round == 1) {
        Common::showBossText(scene, "You don't know who I am, do you?");
    }
    else if (round == 2) {
        Common::showBossText(scene, "Next time we meet, I won't hold back...");
    }
    else if (round == 3) {
        Common::showBossText(scene, "Ha! Ha! Ha!");
    }
    else {
        Common::showBossText(scene, "I will come back...");
    }
    
    Effect::soundCreepyLaughter();

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

    auto animate = Animate::create(Common::createAnimation("troll_dying_", 14, 0.05));
    auto callback2 = [this]() {
        if (sprite != nullptr) {
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap,10);
            BaseNode::destroyNode();
        }
        };
    
    auto callFunc2 = CallFunc::create(callback2);
    
    auto sequence = Sequence::create(animate, callFunc2, nullptr);
    sprite->runAction(sequence);
}
void BossMap3::setHealth(int h) {
    health = h;
};
int BossMap3::getHealth() {
    return health;
}

void BossMap3::updateHealth(int damage) {
    health -= damage;
    updateHealthBar(health);
    hurt();
    if (health <= 0) {
        healthBar->removeFromParentAndCleanup(true);
        healthBarBg->removeFromParentAndCleanup(true);
        die();
    }
}

void BossMap3::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / maxHealth;

    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(healthRatio * Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY()); // Điều chỉnh chiều rộng thanh máu
}
void BossMap3::createHealthBar() {
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
    healthBar->setAnchorPoint(Vec2(0, 0));
    healthBar->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
}
void BossMap3::updateHealthBarPosition() {
    // Cập nhật sprite
    b2Vec2 position = body->GetPosition();
    healthBar->setPosition(position.x * Constants::PIXELS_PER_METER - healthBar->getContentSize().width / 4 * Common::scaleSizeXY(), position.y * Constants::PIXELS_PER_METER + sprite->getContentSize().height / 2 * Common::scaleSizeXY());
    healthBarBg->setPosition(position.x * Constants::PIXELS_PER_METER - healthBar->getContentSize().width / 4 * Common::scaleSizeXY(), position.y * Constants::PIXELS_PER_METER + sprite->getContentSize().height / 2 * Common::scaleSizeXY());
}

void BossMap3::update(float dt) {
    if (isALive) {
        updateHealthBarPosition();

        // Cập nhật thời gian đã trôi qua
        if (isALive && body != nullptr) {
            timeSinceLastAttack += dt;

            if (timeSinceLastAttack >= attackCooldown) {
                canAttack = true;
            }
            if (canAttack) {
                followPlayer();
                //if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {
                //    //hit();
                //    callBat();
                //    // Reset thời gian và cờ tấn công
                //    timeSinceLastAttack = 0.0f;
                //    canAttack = false;
                //}
                if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {
                    //hit();
                    callBat();
                    // Reset thời gian và cờ tấn công
                    timeSinceLastAttack = 0.0f;
                    canAttack = false;
                }
            }
        }
    }
}
void BossMap3::followPlayer() {
    walk();
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();

    if (direction.x < 0) sprite->setScaleX(-scale);
    else sprite->setScaleX(scale);
    float speedAttack = 8.0f;  // Tốc độ di chuyển của enemy
    b2Vec2 velocity = Common::scaleSizeXY() * speedAttack * direction;
    body->SetLinearVelocity(velocity);
}

void BossMap3::hit() {
    if (isHit) return;

    if (sprite != nullptr) {
        sprite->stopAllActions();
        body->SetLinearVelocity(b2Vec2_zero);
        auto animate = Animate::create(Common::createAnimation("troll_attacking_", 11, 0.015));
        Effect::soundMagicFire();

        auto callback = [this]() {
            if (sprite != nullptr) {
                int check = 1;
                // check huong nhan vat
                if (sprite->getScaleX() < 0) {
                    check = -1;
                }
                Fire* fire = new Fire(world, scene, Vec2(sprite->getPositionX() + check * 800 * Common::scaleSizeXY(), sprite->getPositionY() - 80 * Common::scaleSizeXY()), bodyToSpriteMap);
                fire->init();
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

void BossMap3::callBat() {
    if (isHit) return;
    body->SetLinearVelocity(b2Vec2_zero);

    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("troll_taunt_", 17, 0.015));

        auto callback = [this]() {
            if (sprite != nullptr) {
                int check = 1;
                // check huong nhan vat
                if (sprite->getScaleX() < 0) {
                    check = -1;
                }
                Bat* w = new Bat(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY() + 400 *Common::scaleSizeXY()), bodyToSpriteMap);
                w->player = player;
                w->init();
                w->attackRange = 1000;
                idle();
            }
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
}