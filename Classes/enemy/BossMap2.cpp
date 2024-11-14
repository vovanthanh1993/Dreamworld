#include "BossMap2.h"
#include "npc/NPC1.h"
#include "map/port.h"
#include "skill/skull.h"
#include "skill/BoneRain.h"
#include "main/Effect.h"

BossMap2::BossMap2(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) :BaseCharacter(world, scene, position, _bodyToSpriteMap) {
};

void BossMap2::init() {
    spriteNode = SpriteBatchNode::create("Enemy/Bossmap2/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemy/Bossmap2/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wraith_1_Moving Forward_0.png");
    sprite->setScale(Constants::BOSSMAP2_SCALE * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP2);
    
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
    fixtureDef.filter.maskBits = Constants::CATEGORY_STICK| Constants::CATEGORY_WALL| Constants::CATEGORY_LIMIT| Constants::CATEGORY_PLAYER| Constants::CATEGORY_SLASH;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    b2Vec2 velocity(Constants::SPEED_BOSS2 * Common::scaleSizeXY(), 0);
    body->SetGravityScale(0.0f);
    body->SetLinearVelocity(velocity);
    sprite->setScaleX(-Constants::BOSSMAP1_SCALE * Common::scaleSizeXY());
    (*_bodyToSpriteMap)[body] = sprite;
    createHealthBar();
    walk();
}
void BossMap2::idle() {
    if (sprite != nullptr) {
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

    // auto callFunc1 = CallFunc::create(callback1);
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
    Common::changeSpriteColor(sprite, isHit);
}

void BossMap2::walk() {
    if (sprite != nullptr) {
        //sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Wraith_1_Moving Forward_", 11, 0.04));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    }  
}

void BossMap2::die() {
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

    auto animate = Animate::create(Common::createAnimation("Wraith_1_Dying_", 14, 0.05));
    auto callback2 = [this]() {
        if (sprite != nullptr) {
            Gem gem;
            Vec2 pos = sprite->getPosition();
            gem.init(world, scene, pos, _bodyToSpriteMap, 10);
            sprite->removeFromParentAndCleanup(true);
            body->SetUserData(nullptr);
            (*_bodyToSpriteMap).erase(body);
            world->DestroyBody(body);
        }
        };
    
    
   
    auto callFunc2 = CallFunc::create(callback2);
    
    auto sequence = Sequence::create(animate, callFunc2, nullptr);
    sprite->runAction(sequence);
}

void BossMap2::skill2() {

    // Run animation with a callback
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Wraith_1_Casting Spells_", 10, 0.01));
        Effect::soundDarkMagic();
        auto callback2 = [this](){
            int start = -50;
            for (int i = 1; i <= 21; i++) {
                BoneRain* rain = new BoneRain();
                if (sprite != nullptr) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }

                    rain->init(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY()), _bodyToSpriteMap);

                    b2Vec2 velocity(start * Common::scaleSizeXY(), -20 * Common::scaleSizeXY());
                    rain->getBody()->SetLinearVelocity(velocity);
                    start += 5;
                }
            }
            
            };        

       // auto callFunc1 = CallFunc::create(callback1);
        auto callFunc2 = CallFunc::create(callback2);
       
        auto sequence = Sequence::create(animate, callFunc2, nullptr);
        sprite->runAction(sequence);
    }
}

void BossMap2::updateAttack(Player* player, float dt, TMXTiledMap* map) {
    // Cập nhật thời gian đã trôi qua
    if (body != nullptr) {
        timeSinceLastAttack += dt;

        if (timeSinceLastAttack >= attackCooldown) {
            canAttack = true;
        }
        if (canAttack) {
            if (b2Distance(body->GetPosition(), player->getBody()->GetPosition()) <= Constants::ATTACK_RANGE_BOSS_MAP2) {

                // Attack logic
                if (count++ % 3 != 0 && !isInPoint)
                    skill2();
                else {
                    if (!isInPoint) {
                        moveBodyToPoint(map);
                        isInPoint = true;
                        throwSkull();
                        
                    }
                   
                    if (countPhase2++ % 5 == 0) {
                        
                        isInPoint = false;
                        moveBodyToInit(map);
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
void BossMap2::moveBodyToPoint(TMXTiledMap* map) {
    int i = Common::randomNum(1, 3);
    int count = 0;
        auto box = map->getLayer("move");
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

void BossMap2::moveBodyToInit(TMXTiledMap* map) {
    auto box = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = box->getTileAt(Vec2(x, y));
            if (tile) {
                float newAngle = 0.0f;
                body->SetTransform(b2Vec2((x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2) / Constants::PIXELS_PER_METER* Common::scaleSizeXY(), (map->getMapSize().height - y) * Constants::TITLE_SIZE / Constants::PIXELS_PER_METER * Common::scaleSizeXY()), newAngle);

            }
        }
    }
}

void BossMap2::throwSkull() {
    // Run animation with a callback
    if (sprite != nullptr) {
        auto animate = Animate::create(Common::createAnimation("Wraith_1_Casting Spells_", 10, 0.01));
        Effect::soundCreepyLaughter();

        auto callback2 = [this]() {
                Skull* skull1 = new Skull();
                Skull* skull2 = new Skull();
                if (sprite != nullptr) {
                    int check = 1;
                    // check huong nhan vat
                    if (sprite->getScaleX() < 0) {
                        check = -1;
                    }

                    skull1->init(world, scene, Vec2(sprite->getPositionX(), sprite->getPositionY()), _bodyToSpriteMap);
                    b2Vec2 velocity(-10* Common::scaleSizeXY(), -60* Common::scaleSizeXY());
                    skull1->getBody()->SetLinearVelocity(velocity);
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