#include "Hedgehog.h"
Hedgehog::Hedgehog(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {
};

bool Hedgehog::init(Vec2 position) {
    attackCooldown = 0.0f;  // Thời gian chờ giữa các đợt tấn công
    timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
    canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
    isAlive = true;
    scale = 0.4f;
    attackRange = 15;
    health = 13;
    direction = -1;
    speed = 50;

    spriteNode = SpriteBatchNode::create("enemy/hedgehog/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/hedgehog/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Hed_idle_0.png");
    sprite->setScale(scale* Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_HED);
    
    //int* userData = new int(-1);
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
    fixtureDef.density = 100000.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_PLAYER| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    sprite->setScaleX(direction* scale * Common::scaleSizeXY());
    (*bodyToSpriteMap)[body] = sprite;
    idle();

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "hedgehog");
    scene->addChild(this);

    return true;
}
void Hedgehog::idle() {
    if (isAlive) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("Hed_idle Blinking_", 11, 0.04));
        animate->retain();
        sprite->runAction(RepeatForever::create(animate));
    }
    
}

void Hedgehog::walk() {
    if (isAlive) {
        b2Vec2 velocity(direction * speed * Common::scaleSizeXY(), 0);
        body->SetLinearVelocity(velocity);
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("Hed_walking_", 17, 0.04));
        animate->retain();
        sprite->runAction(RepeatForever::create(animate));
    }
    
}

void Hedgehog::die() {
    isAlive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Hed_smoke_", 9, 0.05));
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
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 3));
            BaseNode::destroyNode();
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Hedgehog::update(float dt) {

    // Cập nhật thời gian đã trôi qua
    if (isAlive) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            float dy = body->GetPosition().y - player->getBody()->GetPosition().y;
            float distanceY = std::abs(dy);
            if (distanceY<= 2 && b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= attackRange * Common::scaleSizeXY()) {
                walk();
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
            }
        }
    }
}

void Hedgehog::getDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        die();
        return;
    }
    hurt();
}

void Hedgehog::hurt() {
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Hed_hurt_", 11, 0.04));
    animate->setTag(4);
    auto callback = [this]() {
            idle();
    };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

