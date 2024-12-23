#include "BossMap1.h"

BossMap1::BossMap1(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
};

bool BossMap1::init(Vec2 position) {
    attackCooldown = 2.0f;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    health = 200;
    scale = 1;
    attackRange = 1000;
    isAlive = true;
    direction = -1;
    speed = 10;

    spriteNode = SpriteBatchNode::create("Enemy/Bossmap1/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap1/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("0_boss_idle_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP1);
  
    sprite->setUserData(this);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;

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
    b2Vec2 velocity(direction* speed * Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(direction* scale * Common::scaleSizeXY());
    (*bodyToSpriteMap)[body] = sprite;
    
    stoneBallPool->createPool(world, scene, bodyToSpriteMap, 5);

    createHealthBar();
    walk();

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "bossmap1");
    scene->addChild(this);
    return true;
}
void BossMap1::idle() {
    if (isAlive) {
        b2Vec2 velocity(0, 0);
        body->SetLinearVelocity(velocity);
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("0_boss_idle_", 19, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void BossMap1::walk() {
    if (isAlive) {
        auto animateW = Animate::create(Common::createAnimation("0_boss_walk_", 19, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void BossMap1::die() {
    isAlive = false;

    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    Effect::soundLaughter();
    Common::showTextRandom(scene, dieTextVector, 5);

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
            if (!isAlive) {
                Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, 10);
                Common::spawnCharm(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1,2));
                BaseNode::destroyNode();
            }
        };
    auto callFunc2 = CallFunc::create(callback2);
    
    auto sequence = Sequence::create(animate, callFunc2, nullptr);
    sprite->runAction(sequence);
}

void BossMap1::throwStoneBall() {
    if (isHit) return;
   
    // Run animation with a callback
    if (isHit) return;
    if (isAlive) {
        auto animate = Animate::create(Common::createAnimation("0_boss_attack_", 19, 0.02));
        Effect::soundMagicFire();
        auto callback2 = [this]() {
            createStoneBall();
        };
        auto callFunc2 = CallFunc::create(callback2);

        auto sequence = Sequence::create(animate, callFunc2, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap1::charge() {
    Common::showTextRandom(scene, hugTextVector, 5);

    // Run animation with a callback
    if (isHit) return;
    if (isAlive) {
        auto animate = Animate::create(Common::createAnimation("0_boss_specialty_", 19, 0.02));
        Effect::soundMagicFire();
        followPlayer();
        body->SetLinearVelocity(b2Vec2(direction * 60 * Common::scaleSizeXY(), 0));

        // De va cham voi player
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            // Lấy dữ liệu bộ lọc hiện tại
            b2Filter filter = fixture->GetFilterData();
            // Cập nhật categoryBits và maskBits
            filter.categoryBits = Constants::CATEGORY_ENEMY;
            filter.maskBits = Constants::CATEGORY_STICK | Constants::CATEGORY_WALL | Constants::CATEGORY_LIMIT | Constants::CATEGORY_SLASH| Constants::CATEGORY_PLAYER;
            // Thiết lập lại dữ liệu bộ lọc
            fixture->SetFilterData(filter);
        }
        auto callback1 = [this]() {
            // De va cham voi player
            for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                // Lấy dữ liệu bộ lọc hiện tại
                b2Filter filter = fixture->GetFilterData();
                // Cập nhật categoryBits và maskBits
                filter.categoryBits = Constants::CATEGORY_ENEMY;
                filter.maskBits = Constants::CATEGORY_STICK | Constants::CATEGORY_WALL | Constants::CATEGORY_LIMIT | Constants::CATEGORY_SLASH;
                // Thiết lập lại dữ liệu bộ lọc
                fixture->SetFilterData(filter);
            }
        };
        auto callFunc1 = CallFunc::create(callback1);
        //auto callFunc2 = CallFunc::create(callback2);
        
        auto sequence = Sequence::create(animate, callFunc1, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap1::setHealth(int h) {
    health = h;
};
int BossMap1::getHealth() {
    return health;
}

void BossMap1::getDamage(int damage) {
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

void BossMap1::createStoneBall() {
    Common::showTextRandom(scene, throwStoneTextVector, 5);
    StoneBall* stoneball = stoneBallPool->getFromPool();
    if (stoneball != nullptr) {
        stoneball->init(Vec2(sprite->getPositionX(), sprite->getPositionY() + 200 * Common::scaleSizeXY()));
        stoneBallVector.push_back(stoneball);
        stoneball->getBody()->SetLinearVelocity(b2Vec2(direction * 20 * Common::scaleSizeXY(), 60 * Common::scaleSizeXY()));
    }
}

void BossMap1::followPlayer() {
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();
    int check = 1;

    if (direction.x < 0) {
        this->direction = -1;
        sprite->setScaleX(-scale);
    }
    else {
        this->direction = 1;
        sprite->setScaleX(scale);
    }
}

void BossMap1::update(float dt) {

    if (isAlive) {
        updateHealthBarPosition();

        if (isHit) return;
        // Cập nhật thời gian đã trôi qua
        if (body != nullptr) {
            timeSinceLastAttack += dt;
            if (timeSinceLastAttack >= attackCooldown) {
                if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {

                    // Attack logic
                    if (count++ % 3 != 0) 
                        throwStoneBall();
                    else
                        charge();
                    // Reset thời gian và cờ tấn công
                    timeSinceLastAttack = 0.0f;
                }
            }
        }
    }
}