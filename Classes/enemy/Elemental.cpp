#include "Elemental.h"
#include "Effect.h"
#include "Rain.h"
Elemental::Elemental(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) :BaseCharacter(world, scene, position, _bodyToSpriteMap) {
};

void Elemental::init() {
    spriteNode = SpriteBatchNode::create("Enemy/Elemental/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Elemental/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Elemental_1_Idle_0.png");
    sprite->setScale(Common::WRAITH_SCALE * Common::scaleSprite());
    sprite->setTag(Common::TAG_ENEMY);

    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode);

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
    fixtureDef.filter.maskBits = Common::CATEGORY_WALL | Common::CATEGORY_LIMIT | Common::CATEGORY_PLAYER | Common::CATEGORY_SLASH;//Common::CATEGORY_STICK| ;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    body->SetGravityScale(0.0f);
    sprite->setScaleX(-Common::WRAITH_SCALE * Common::scaleSprite());
    (*_bodyToSpriteMap)[body] = sprite;
    idle();
    
}
void Elemental::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Elemental_1_Idle_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
}

void Elemental::walk() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Elemental_1_Moving Forward_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}


void Elemental::hit() {
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Elemental_1_Casting Spells_", 17, 0.01));

        auto callback2 = [this]() {
            int start = -50;
            for (int i = 1; i <= 11; i++) {
                Rain* rain = new Rain();
                if (sprite != nullptr) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }

                    rain->init(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY()), _bodyToSpriteMap);
                    rain->getSprite()->setScaleX(check * rain->getSprite()->getScale());

                    b2Vec2 velocity(start * Common::scaleSprite(), -20 * Common::scaleSprite());
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

void Elemental::updateAttack(Player* player, float dt) {
    // Cập nhật thời gian đã trôi qua
    if (body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Common::ATTACK_RANGE_WRAITH * Common::scaleSprite()) {

                //// Attack logic
                //hit(world, scene, _bodyToSpriteMap);
                //// Reset thời gian và cờ tấn công
                //timeSinceLastAttack = 0.0f;
                //canAttack = false;
            }
        }
    }
    
}

void Elemental::die() {
    b2Vec2 velocity(0, 0);
    body->SetLinearVelocity(velocity);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Elemental_1_Dying_", 14, 0.05));
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
            gem->init(world, scene, sprite->getPosition(), _bodyToSpriteMap, 1);
            body->SetUserData(nullptr);
            (*_bodyToSpriteMap).erase(body);
            world->DestroyBody(body);
        }
        };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}
