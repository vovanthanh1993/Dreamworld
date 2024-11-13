#include "enemy/Warrior.h"
#include "main/Effect.h"

Warrior::Warrior(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) :BaseCharacter(world, scene, position, _bodyToSpriteMap) {
};

void Warrior::init() {
    spriteNode = SpriteBatchNode::create("enemy/warrior/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy/warrior/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Idle_0.png");
    sprite->setScale(Common::WARRIOR_SCALE* Common::scaleSprite());
    sprite->setTag(Common::TAG_ENEMY);
    
    int* userData = new int(-1);
    sprite->setUserData(userData);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 1);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER, sprite->getPositionY() / Common::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    //bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Common::CATEGORY_STICK| Common::CATEGORY_WALL| Common::CATEGORY_LIMIT| Common::CATEGORY_PLAYER| Common::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(Common::SPEED_ENEMY* Common::scaleSprite(), 0);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(-Common::WARRIOR_SCALE* Common::scaleSprite());
    (*_bodyToSpriteMap)[body] = sprite;
}
void Warrior::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Idle_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Warrior::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Walking_", 17, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void Warrior::die() {
    isLive = false;
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Dying_", 14, 0.05));
    Effect::enemyDie();

    // Lặp qua tất cả các fixture của body
    for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        // Lấy dữ liệu bộ lọc hiện tại
        b2Filter filter = fixture->GetFilterData();

        // Cập nhật categoryBits và maskBits
        filter.categoryBits = Common::CATEGORY_ENEMY;
        filter.maskBits = Common::CATEGORY_WALL;

        // Thiết lập lại dữ liệu bộ lọc
        fixture->SetFilterData(filter);
    }
   

    auto callback = [this]() {
        if (sprite != nullptr) {
            sprite->removeFromParentAndCleanup(true);
            Gem* gem = new Gem();
            gem->init(world, scene, sprite->getPosition(), _bodyToSpriteMap, Common::randomNum(1, 3));
            body->SetUserData(nullptr);
            (*_bodyToSpriteMap).erase(body);
            world->DestroyBody(body);
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

SlashEnemy* Warrior::hit() {

    // Run animation with a callback
    SlashEnemy* slashEnemy = new SlashEnemy();
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("Attacking_", 11, 0.015));

        
        auto callback = [this, slashEnemy]() {
            if (sprite != nullptr) {
                int check = 1;
                // check huong nhan vat
                if (sprite->getScaleX() < 0) {
                    check = -1;
                }

                slashEnemy->init(world, scene, Vec2(sprite->getPositionX() + check * 120 * Common::PLAYER_SCALE* Common::scaleSprite(), sprite->getPositionY()));
                slashEnemy->getSprite()->setScaleX(check * Common::STICK_SCALE* Common::scaleSprite());
                walk();
            }
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
    
    return slashEnemy;
}

void Warrior::updateAttack(vector<SlashEnemy*> &slashEnemyVector, Player* player, float dt) {
    // Cập nhật thời gian đã trôi qua
    if (body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Common::ATTACK_RANGE_WAR* Common::scaleSprite()) {

                // Attack logic
                slashEnemyVector.push_back(hit());
                // Reset thời gian và cờ tấn công
                timeSinceLastAttack = 0.0f;
                canAttack = false;
            }
        }
    }
}
