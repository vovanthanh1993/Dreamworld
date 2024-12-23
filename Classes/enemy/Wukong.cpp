#include "Wukong.h"
Wukong::Wukong(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
};

bool Wukong::init(Vec2 position) {
    attackCooldown = 1.5f;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    attackRange = 3;
    scale = 0.6;
    isAlive = true;
    isActive = true;
    canAttack = false;
    direction = -1;
    speed = 10;
    health = 11;

    spriteNode = SpriteBatchNode::create("enemy/wukong/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/wukong/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("ew-Idle_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_WUKONG);
    
    sprite->setUserData(this);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 1);

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
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(direction*speed* Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(direction * scale * Common::scaleSizeXY());
    (*bodyToSpriteMap)[body] = sprite;
    walk();
    slashEnemyPool->createPool(world, scene, bodyToSpriteMap, 5);

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "Wukong");
    scene->addChild(this);

    return true;
}
void Wukong::idle() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("ew-Idle_", 9, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Wukong::walk() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("ew-Walk_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void Wukong::die() {
    isAlive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("ew-Death_", 25, 0.01));
    Effect::enemyDie();

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

    auto callback = [this]() {
        if (!isAlive) {
            //Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 3));
            BaseNode::destroyNode();
            this->removeFromParentAndCleanup(true);
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Wukong::hit() {

    // Run animation with a callback
    
    if (isAlive) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("ew-Hit_", 21, 0.015));

        auto callback = [this]() {
            if (isAlive) {
                SlashEnemy* slashEnemy = slashEnemyPool->getFromPool();
                if (slashEnemy != nullptr) {
                    slashEnemy->init(Vec2(sprite->getPositionX() + direction * 60 * Common::scaleSizeXY(), sprite->getPositionY()));
                    slashEnemy->getSprite()->setScaleX(direction * slashEnemy->getScale() * Common::scaleSizeXY());
                }
                walk();
            }
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
}
void Wukong::update(float dt) {
    
    // Cập nhật thời gian đã trôi qua
    if (isAlive) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            float dy = body->GetPosition().y - player->getBody()->GetPosition().y;
            float distanceY = std::abs(dy);
            if(isFollowPlayer && distanceY < 2) followPlayer();
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {
                 hit();
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
            }
        }
    }
}

void Wukong::followPlayer() {
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();

    if (direction.x < 0) {
        sprite->setScaleX(-scale);
        this->direction = -1;
        direction = b2Vec2(-1, -10);
    }
    else {
        sprite->setScaleX(scale);
        this->direction = 1;
        direction = b2Vec2(1, -10);
    }
    b2Vec2 velocity = Common::scaleSizeXY() * speed * direction;
    body->SetLinearVelocity(velocity);
}

void Wukong::getDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        die();
        return;
    }
    hurt();
}

void Wukong::hurt() {
    Effect::enemyHurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("ew-GetHit_", 13, 0.02));
    animate->setTag(4);
    auto callback = [this]() {
        walk();
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

