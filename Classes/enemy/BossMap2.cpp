#include "BossMap2.h"

BossMap2::BossMap2(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
};

bool BossMap2::init(Vec2 position) {
    attackCooldown = 0.5f;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    health = 200;
    scale = 1;
    direction = -1;
    speed = 15;
    isAlive = true;

    spriteNode = SpriteBatchNode::create("Enemy/Bossmap2/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap2/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wraith_1_Moving Forward_0.png");
    sprite->setScale(scale*Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP2);
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
    b2Vec2 velocity(direction* speed * Common::scaleSizeXY(), 0);
    body->SetGravityScale(0.0f);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(direction* scale * Common::scaleSizeXY());
    (*bodyToSpriteMap)[body] = sprite;
    createHealthBar();
    walk();

    boneRainPool->createPool(world, scene, bodyToSpriteMap, 10);
    warriorPool->createPool(world, scene, bodyToSpriteMap, 10);
    

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "bossmap2");
    scene->addChild(this);
    return true;
}
void BossMap2::idle() {
    if (isAlive) {
        b2Vec2 velocity(0, 0);
        body->SetLinearVelocity(velocity);
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("0_boss_idle_", 19, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void BossMap2::hurt() {

    Effect::soundBoss1Hurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wraith_1_Hurt_", 11, 0.04));
    auto callback = [this]() {
        walk();
    };

    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
    Common::changeSpriteColor(sprite, isHit);
}

void BossMap2::walk() {
    if (isAlive) {
        auto animateW = Animate::create(Common::createAnimation("Wraith_1_Moving Forward_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void BossMap2::die() {
    
    for (Warrior* war : warriorPool->getPool()) {
        if (war->isAlive) {
            war->die();
        }
    }

    isAlive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    Common::showTextRandom(scene, dieTextVector, 5);
    
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

    auto animate = Animate::create(Common::createAnimation("Wraith_1_Dying_", 14, 0.05));
    auto callback2 = [this]() {
            if (!isAlive) {
                Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap,10);
                Common::spawnCharm(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 3));
                BaseNode::destroyNode();
            }
        };
    
    auto callFunc2 = CallFunc::create(callback2);
    
    auto sequence = Sequence::create(animate, callFunc2, nullptr);
    sprite->runAction(sequence);
}

void BossMap2::boneRain() {

    // Run animation with a callback
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Wraith_1_Casting Spells_", 10, 0.01));
        Effect::soundDarkMagic();
        auto callback2 = [this](){
            BoneRain* rain = boneRainPool->getFromPool();
            if (rain != nullptr) {
                rain->player = player;
                rain->init(Vec2(sprite->getPositionX(), sprite->getPositionY()));
            }
        };        

        auto callFunc2 = CallFunc::create(callback2);
        auto sequence = Sequence::create(animate, callFunc2, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap2::update(float dt) {
    if (isAlive) {
        updateHealthBarPosition();

        // Cập nhật thời gian đã trôi qua
        if (body != nullptr) {
            timeSinceLastAttack += dt;

            if (timeSinceLastAttack >= attackCooldown) {
                canAttack = true;
            }
            if (canAttack) {
                if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Constants::ATTACK_RANGE_BOSS_MAP2) {

                    // Attack logic
                    
                    if (count++ < 10 && countPhase2 == 1)
                        boneRain();
                    else {
                        if (countPhase2 == 1) {
                            moveBodyToPoint();
                            throwWarrior();
                        }

                        if (countPhase2 == 10) {
                            throwWarrior();
                        }

                        // Ve lai vi tri dau tien
                        if (countPhase2++ == 15) {
                            moveBodyToInit();
                            countPhase2 = 1;
                            count = 1;
                        }
                        
                    }

                    // Reset thời gian và cờ tấn công
                    timeSinceLastAttack = 0.0f;
                    canAttack = false;
                }
            }
        }
    }
}
void BossMap2::setHealth(int h) {
    health = h;
};
int BossMap2::getHealth() {
    return health;
}

void BossMap2::updateHealth(int damage) {
        health -= damage;
        updateHealthBar(health);
        hurt();
        if (health <= 0) {
            healthBar->removeFromParentAndCleanup(true);
            healthBarBg->removeFromParentAndCleanup(true);
            die();
        } 
}
void BossMap2::moveBodyToPoint() {
    int i = Common::randomNum(1, 3);
    int count = 0;
        auto box = map->getLayer("point");
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = box->getTileAt(Vec2(x, y));
                if (tile) {
                   count++;
                   if (i == count) {
                       float newAngle = 0.0f;
                       body->SetTransform(b2Vec2((x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2) / Constants::PIXELS_PER_METER * Common::scaleSizeXY(), (map->getMapSize().height - y) * Constants::TITLE_SIZE / Constants::PIXELS_PER_METER * Common::scaleSizeXY()), newAngle);
                   }
                   
                   
                }
            }
        }
}

void BossMap2::moveBodyToInit() {
    Common::showTextRandom(scene, initTextVector, 5);
    auto boss = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = boss->getTileAt(Vec2(x, y));
            if (tile) {
                float newAngle = 0.0f;
                body->SetTransform(b2Vec2((x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2) / Constants::PIXELS_PER_METER* Common::scaleSizeXY(), (map->getMapSize().height - y) * Constants::TITLE_SIZE / Constants::PIXELS_PER_METER * Common::scaleSizeXY()), newAngle);

            }
        }
    }
}

void BossMap2::throwWarrior() {

    Common::showTextRandom(scene, throwWarTextVector, 5);

    // Run animation with a callback
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Wraith_1_Casting Spells_", 10, 0.01));
        Effect::soundCreepyLaughter();

        auto callback2 = [this]() {
                
                if (isAlive) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }
                    Warrior* w = warriorPool->getFromPool();
                    if (w != nullptr) {
                        w->player = player;
                        w->init(sprite->getPosition());
                        w->speed = 8;
                        w->isFollowPlayer = true;
                        w->followPlayer();
                    }
                }
            };

        // auto callFunc1 = CallFunc::create(callback1);
        auto callFunc2 = CallFunc::create(callback2);

        auto sequence = Sequence::create(animate, callFunc2, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap2::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / maxHealth;

    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(healthRatio * Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY()); // Điều chỉnh chiều rộng thanh máu
}
void BossMap2::createHealthBar() {
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
void BossMap2::updateHealthBarPosition() {
    // Cập nhật sprite
    b2Vec2 position = body->GetPosition();
    healthBar->setPosition(position.x * Constants::PIXELS_PER_METER - healthBar->getContentSize().width / 4 * Common::scaleSizeXY(), position.y * Constants::PIXELS_PER_METER + sprite->getContentSize().height / 2 * Common::scaleSizeXY());
    healthBarBg->setPosition(position.x * Constants::PIXELS_PER_METER -healthBar->getContentSize().width / 4 * Common::scaleSizeXY(), position.y * Constants::PIXELS_PER_METER + sprite->getContentSize().height / 2 * Common::scaleSizeXY());
}