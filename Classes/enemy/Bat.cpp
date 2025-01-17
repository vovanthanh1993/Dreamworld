#include "Bat.h"
Bat::Bat(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
     
};

bool Bat::init(Vec2 position) {
    attackCooldown = 0;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    scale = 0.4;
    health = 1;
    attackRange = 20;
    direction = 1;
    speed = 6;
    isActive = true;
    isAlive = true;

    spriteNode = SpriteBatchNode::create("enemy/Bat/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/Bat/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("bat_idle Blinking_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BAT);

    sprite->setUserData(this);
    sprite->setPosition(position);
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
    fixtureDef.density = 1000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_LIMIT|Constants::CATEGORY_SLASH| Constants::CATEGORY_PLAYER;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(direction * speed * Common::scaleSizeXY(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(direction * scale * Common::scaleSizeXY());
    body->SetGravityScale(0.0f);
    (*bodyToSpriteMap)[body] = sprite;
    idle();

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "Bat");
    scene->addChild(this);

    return true;
}
void Bat::idle() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("bat_idle Blinking_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Bat::die() {
    isAlive = false;
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

    
    
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("bat_smoke_", 9, 0.04));
    MusicManager::getInstance()->enemyDie();

    auto callback = [this]() {
        if (!isAlive) {
            BaseNode::destroyNode();
            this->removeFromParentAndCleanup(true);
        }
    };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Bat::update(float dt) {

    // Cập nhật thời gian đã trôi qua
    if (isAlive && body != nullptr) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {
                if (!canAttack) {
                    // Lặp qua tất cả các fixture của body
                    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                        // Lấy dữ liệu bộ lọc hiện tại
                        b2Filter filter = fixture->GetFilterData();

                        // Cập nhật categoryBits và maskBits
                        filter.categoryBits = Constants::CATEGORY_ENEMY;
                        filter.maskBits = Constants::CATEGORY_STICK | Constants::CATEGORY_SLASH | Constants::CATEGORY_PLAYER;

                        // Thiết lập lại dữ liệu bộ lọc
                        fixture->SetFilterData(filter);
                    }
                }

                followPlayer();
                canAttack = true;
            }
    }
}

void Bat::followPlayer() {
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();

    if (direction.x < 0) sprite->setScaleX(-scale * Common::scaleSizeXY());
    else sprite->setScaleX(scale * Common::scaleSizeXY());
    float speedAttack = 8.0f;  // Tốc độ di chuyển của enemy
    b2Vec2 velocity = Common::scaleSizeXY() * speedAttack * direction;
    body->SetLinearVelocity(velocity);
}

void Bat::getDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        die();
        return;
    }
    hurt();
}

void Bat::hurt() {
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("bat_hurt_", 11, 0.04));
    animate->setTag(4);
    auto callback = [this]() {
        idle();
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}