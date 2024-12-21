#include "BossMap4.h"
BossMap4::BossMap4(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseEnemy(world, scene, bodyToSpriteMap) {};
bool BossMap4::init(Vec2 position) {
    scale = 0.3;
    isActive = true;
    health = 100;
    isAlive = true;

    sprite = Sprite::create("enemy/bossmap4/sprite.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_BOSSMAP4);
    scene->addChild(sprite);
    sprite->setUserData(this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(position.x / Constants::PIXELS_PER_METER, position.y / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

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
    fixtureDef.filter.maskBits = Constants::CATEGORY_SLASH | Constants::CATEGORY_STICK;

    // Gán fixture cho body
    body->SetGravityScale(0.0f);
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    Common::zoomAction(sprite);

    // Lên lịch gọi update mỗi frame
    // Schedule the spawnEnemies function every 5 seconds
    wukongPool = new WukongPool(world, scene, bodyToSpriteMap, 6);
    this->schedule([this](float dt) {
        spawnWukong();
        }, 3, "spawn_wukong");

    //// Pool for bat
    wukongFlyPool = new WukongFlyPool(world, scene, bodyToSpriteMap, 10);
    this->schedule([this](float dt) {
        spawnWukongFly();
        },5 , "spawn_wukong_fly");
    scene->addChild(this);
    return true;
}

void BossMap4::getDamage(int damage) {
    health -= damage;
    hurt();
    
    
    if (health <= 0) {
        /*healthBar->removeFromParentAndCleanup(true);
        healthBarBg->removeFromParentAndCleanup(true);*/
        die();

    }
}

void BossMap4::hurt() {
    Common::changeSpriteColor(sprite, isHit);
}

void BossMap4:: die() {
    this->unschedule("spawn_wukong");
    this->unschedule("spawn_wukong_fly");
    for (Wukong* w : wukongPool->getPool()) {
        if (w->isAlive) {
            w->die();
        }
    }
    for (WukongFly* w : wukongFlyPool->getPool()) {
        if (w->isAlive) {
            w->die();
        }
    }

    isAlive = false;
    /*auto animate = Animate::create(Common::createAnimation("0_boss_die_", 19, 0.05));*/
    Effect::smoke(world, scene, sprite->getPosition());
    auto callback2 = [this]() {
        if (!isAlive) {
            Common::spawnGem(world, scene, sprite->getPosition(), bodyToSpriteMap, 10);
            Common::spawnCharm(world, scene, sprite->getPosition(), bodyToSpriteMap, Common::randomNum(1, 2));
            BaseNode::destroyNode();
        }
        };
    auto callFunc2 = CallFunc::create(callback2);

    auto sequence = Sequence::create(callFunc2, nullptr);
    sprite->runAction(sequence);
}

void BossMap4::spawnWukongFly() {
    // spawn bat
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int i = Common::randomNum(1, 4);
    int count = 0;
    auto layer = map->getLayer("wukongfly");
    if (layer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = layer->getTileAt(Vec2(x, y));
                if (tile) {
                    if (i == ++count) {
                        WukongFly* w = wukongFlyPool->getFromPool();
                        if (w != nullptr) {
                            w->player = player;
                            w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                            w->attackRange = 1000;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void BossMap4::spawnWukong() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto wukongLayer = map->getLayer("wukong");
    if (wukongLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wukongLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    Wukong* w = wukongPool->getFromPool();
                    if (w != nullptr) {
                        w->player = player;
                        w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                        break;
                    }
                }
            }
        }
    }
}