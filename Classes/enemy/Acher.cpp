#include "Acher.h"

Acher::Acher(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};

bool Acher::init() {
    if (!Node::init()) {
        return false;
    }
    spriteNode = SpriteBatchNode::create("Enemy/Acher/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Acher/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("0_Archer_Idle_0.png");
    sprite->setScale(Constants::ACHER_SCALE * Common::scaleSizeXY());
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
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    sprite->setScaleX(-Constants::ACHER_SCALE * Common::scaleSizeXY());
    idle();
    (*bodyToSpriteMap)[body] = sprite;

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "acher");
    scene->addChild(this);
    return true;
}
void Acher::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("0_Archer_Idle_", 9, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Acher::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Walking_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void Acher::hit() {
    // Run animation with a callback
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("0_Archer_Shooting_", 8, 0.05));

        
        auto callback = [this]() {
            if (sprite != nullptr) {
                int check = 1;
                // check huong nhan vat
                if (sprite->getScaleX() < 0) {
                    check = -1;
                }
                Arrow* arrow = new Arrow(world, scene, Vec2(sprite->getPositionX() + check * 60 * Common::scaleSizeXY(), sprite->getPositionY()), bodyToSpriteMap);
                arrow->init();
                arrow->getSprite()->setScaleX(check * Constants::ARROW_SCALE * Common::scaleSizeXY());
                b2Vec2 velocity(40 * check * Common::scaleSizeXY(), 0);
                arrow->getBody()->SetLinearVelocity(velocity);
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
            }
        }
    }
    
}

void Acher::die() {

    isAlive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("0_Archer_Dying_", 14, 0.05));
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
        if (sprite != nullptr) {
            sprite->removeFromParentAndCleanup(true);
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 3));
            BaseNode::destroyNode();
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}
