#include "Acher.h"

Acher::Acher(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
};

bool Acher::init(Vec2 position) {
    attackCooldown = 2.5f;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    isAlive = true;
    attackRange = 15;
    direction = -1;
    health = 12;
    scale = 0.3;

    if (!Node::init()) {
        return false;
    }
    spriteNode = SpriteBatchNode::create("Enemy/Acher/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Acher/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("archer_Idle Blinking_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_ACHER);
    sprite->setUserData(this);
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
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    sprite->setScaleX(-scale * Common::scaleSizeXY());
    idle();
    (*bodyToSpriteMap)[body] = sprite;

    arrowPool->createPool(world, scene, bodyToSpriteMap, 5);

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "acher");
    scene->addChild(this);
    return true;
}
void Acher::idle() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("archer_Idle Blinking_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Acher::walk() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("archer_walking_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void Acher::hit() {
    // Run animation with a callback
    if (isAlive) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("archer_Shooting_", 8, 0.05));
        auto callback = [this]() {
            if (sprite != nullptr) {
                Arrow* arrow = arrowPool->getFromPool();
                if (arrow != nullptr) {
                    arrow->init(Vec2(sprite->getPositionX() + direction * 60 * Common::scaleSizeXY(), sprite->getPositionY()));
                    arrow->getSprite()->setScaleX(direction * arrow->getScale() * Common::scaleSizeXY());
                    b2Vec2 velocity(40 * direction * Common::scaleSizeXY(), 0);
                    arrow->getBody()->SetLinearVelocity(velocity);
                }
                idle();
            }
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
}

void Acher::update(float dt) {
    // Cập nhật thời gian đã trôi qua
    if (isAlive && body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            float dy = body->GetPosition().y - player->getBody()->GetPosition().y;
            float distanceY = std::abs(dy);
            if (distanceY <= 2 && b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {

                // Attack logic
                hit();
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
                followPlayer();
            }
        }
    }
    
}

void Acher::die() {

    isAlive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("archer_Dying_", 14, 0.05));
    MusicManager::getInstance()->enemyDie();

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
            sprite->removeFromParentAndCleanup(true);
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 3));
            BaseNode::destroyNode();
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Acher::getDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        die();
        return;
    }
    hurt();
}

void Acher::hurt() {
    MusicManager::getInstance()->enemyHurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("archer_Hurt_", 11, 0.04));
    animate->setTag(4);
    auto callback = [this]() {
        idle();
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Acher::followPlayer() {
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();

    if (direction.x < 0) {
        sprite->setScaleX(-scale);
        this->direction = -1;
    }
    else {
        sprite->setScaleX(scale);
        this->direction = 1;
    }
}
