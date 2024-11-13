#include "player/Player.h"
#include "skill/Stick.h"
#include <player/skill/Eagle.h>
#include "scene/GameOver.h"
#include <iostream>
#include <fstream>

Player::Player(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) :BaseCharacter(world, scene, position, _bodyToSpriteMap) {
};
void Player::init(bool isNew) {
    spriteNode = SpriteBatchNode::create("player/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wukong-Idle_0.png");
    sprite->setScale(Common::PLAYER_SCALE*Common::scaleSprite());
    sprite->setPosition(position);
    sprite->setTag(Common::TAG_PLAYER);
    
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 1);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER, sprite->getPositionY() / Common::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_PLAYER;
    fixtureDef.filter.maskBits = Common::CATEGORY_SLASH_ENEMY | Common::CATEGORY_WALL| Common::CATEGORY_ITEM| Common::CATEGORY_CHEST| Common::CATEGORY_ARROW | Common::CATEGORY_BOX| Common::CATEGORY_GEM; //  Không va chạm với CATEGORY_B
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);

    // load from file
    loadPlayerDataInit(isNew);
    scene->addChild(uiNode, 10);
    (*_bodyToSpriteMap)[body] = sprite;
    idle();

    initKeyEvent();
    initMouseEvent();

}
void Player::idle() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Idle_", 9, 0.04));
    animate->retain();
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(2); // Gán tag để quản lý hoạt ảnh
    sprite->runAction(repeatAnimate);
}

void Player::walk() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Walk_", 17, 0.02));
    animate->retain();
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(1);
    sprite->runAction(repeatAnimate);
}

void Player::healing() {
    if (!isAlive) return;
    auto animateW = Animate::create(Common::createAnimation("Wukong-Healing_", 3, 0.1));
    sprite->runAction(animateW);
}

void Player::jump() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Jump_", 9, 0.0001));
    Effect::playerJump();
    b2Vec2 velocity(body->GetLinearVelocity().x, Common::JUMP_PLAYER * Common::scaleSprite());
    body->SetLinearVelocity(velocity);
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(3);
    sprite->runAction(repeatAnimate);
}

Slash* Player::hit() {
    if (!isAlive) return nullptr;
    
    Slash* slash = nullptr;
    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
    // Kiểm tra nếu đang không tấn công hoặc đã qua thời gian chờ
    if ((currentTime - lastAttackTime >= attackCooldown)) {
        sprite->stopAllActions();
        lastAttackTime = currentTime; // Cập nhật thời gian của lần tấn công cuối
        auto animate = Animate::create(Common::createAnimation("Wukong-Hit_", 21, 0.013));
        slash = new Slash();
        
        // Run animation with a callback
        auto callback = [this, slash]() {
            int check = 1;
            // check huong nhan vat
            if (sprite->getScaleX() < 0) {
                check = -1;
            }

            slash->init(world, scene, Vec2(sprite->getPositionX() + check * 100 * Common::PLAYER_SCALE * Common::scaleSprite(), sprite->getPositionY()));
            auto sprite = slash->getSprite();
            sprite->setScaleX(check * Common::STICK_SCALE * Common::scaleXSprite());
            };

        
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sequence->setTag(4);
        sprite->runAction(sequence);
    }
    return slash;
}

void Player::throwStick() {
    if (!isAlive) return;
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Throw_", 20, 0.005));
    if (stickNum > 0) {
        Stick* stick = new Stick();
        int check = 1;
        // check huong nhan vat
        if (this->sprite->getScaleX() < 0) {
            check = -1;
        }

        stick->init(world, scene, Vec2(sprite->getPositionX() + check * 10 * Common::PLAYER_SCALE * Common::scaleSprite(), sprite->getPositionY()),
            _bodyToSpriteMap);
        stick->getSprite()->setScaleX(check * stick->getSprite()->getScale());
        b2Vec2 velocity(40 * check * Common::scaleSprite(), 0);
        stick->getBody()->SetLinearVelocity(velocity);
        updateStickNum(-1);
    }
        animate->setTag(4);
        sprite->runAction(animate);
        
}


void Player::eagle() {
    int manaUse = 5;
    if (!isAlive || mana < manaUse) return;
    useMana(manaUse);
    Effect::eagle();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Throw_", 20, 0.005));

    int check = 1;
    // check huong nhan vat              
    if (this->sprite->getScaleX() < 0) {
        check = -1;
    }

    int y = 0;
    int x = -100 * Common::scaleXSprite();
    for (int i = 1; i <= 3; i++) {
        Eagle* eagle = new Eagle();
        eagle->init(world, scene, Vec2(sprite->getPositionX() + check * x + check * 10 * Common::PLAYER_SCALE * Common::scaleSprite(), sprite->getPositionY() + y),
            _bodyToSpriteMap);
        eagle->getSprite()->setScaleX(check * Common::STICK_SCALE * Common::scaleXSprite());
        b2Vec2 velocity(30 * check * Common::scaleSprite(), 0);
        eagle->getBody()->SetLinearVelocity(velocity);
        y += 20 * Common::scaleYSprite();
        x += 100 * Common::scaleXSprite();
    }
    animate->setTag(4);
    sprite->runAction(animate);
    (*_bodyToSpriteMap)[body] = sprite;
}

void Player::setHealth(int h) {
    health = h;
};
int Player::getHealth() {
    return health;
}
void Player::setStickNum(int h) {
    stickNum = h;
}
int Player::getStickNum() {
    return stickNum;
}
void Player::initHealth() {
    createHealthBar();
    createManaBar();
}

void Player::getDamage(int damage) {
    if (!isAlive) return;
    updateHealth(damage);
}
void Player::healing(int num) {
    healing();
    Effect::soundHealth();
    health += num;
    health = health > maxHealth ? maxHealth : health;
    updateHealthBar(health);
}

void Player::updateStickNum(int stick) {
    stickNum += stick;
    stickNum = stickNum < 0 ? 0 : stickNum;
    stickNum = stickNum >= maxStickNum ? maxStickNum : stickNum;
    stickLabel->setString("x" + to_string(stickNum));
    if (stick > 0) {
        Effect::getStick();
    }
}

void Player::initItem() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    int y = screenSize.height;
    Sprite* head = Sprite::create("ui/head2.png");
    head->setPosition(origin.x, y);
    head->setAnchorPoint(Vec2(0, 1));
    head->setScale(0.8* Common::scaleSprite());
    uiNode->addChild(head);

    int xs = origin.x + 160*Common::scaleSprite();
    //int xs = origin.x + head->getContentSize().width* head->getScale();
    Sprite* backStick = Sprite::create("ui/BackStick.png");
    backStick->setPosition(xs, y - 90 * Common::scaleSprite());
    backStick->setAnchorPoint(Vec2(0, 1));
    uiNode->addChild(backStick);
    backStick->setScale(0.2* Common::scaleSprite());

    xs += 50 * Common::scaleSprite();
    //stickNum = maxStickNum;
    stickLabel = Label::createWithTTF("x" + to_string(stickNum), "fonts/Marker Felt.ttf", 30);
    stickLabel->setPosition(xs, y - 130 * Common::scaleSprite());
    stickLabel->setScale(Common::scaleSprite());
    uiNode->addChild(stickLabel);
    
    xs += 50* Common::scaleSprite();
    Sprite* gourd = Sprite::create("Item/gourd/gourd.png");
    gourd->setAnchorPoint(Vec2(0, 0.5));
    gourd->setPosition(xs, y - 115 * Common::scaleSprite());
    gourd->setScale(0.35* Common::scaleSprite());
    uiNode->addChild(gourd);
    xs += 60 *Common::scaleSprite();
    gourdLabel = Label::createWithTTF("x" + to_string(gourdNum), "fonts/Marker Felt.ttf", 30);
    gourdLabel->setPosition(xs, stickLabel->getPositionY());
    gourdLabel->setScale(Common::scaleSprite());
    uiNode->addChild(gourdLabel);

    xs += 40 * Common::scaleSprite();
    Sprite* gemSprite = Sprite::create("item/gem/gem.png");
    gemSprite->setPosition(xs, y - 100 * Common::scaleSprite());
    gemSprite->setAnchorPoint(Vec2(0, 1));
    uiNode->addChild(gemSprite);
    gemSprite->setScale(0.1* Common::scaleSprite());
    xs += 70 * Common::scaleSprite();
    gemLabel = Label::createWithTTF("x" + to_string(gem), "fonts/Marker Felt.ttf", 30);
    gemLabel->setPosition(xs, stickLabel->getPositionY());
    gemLabel->setScale(Common::scaleSprite());
    uiNode->addChild(gemLabel);
}


void Player::updateGem(int i) {
    gem+=i;
    gemLabel->setString("x" + to_string(gem));
    Effect::soundTing();
}

void Player::updateGourd(int i) {
    gourdNum += i;
    gourdLabel->setString("x" + to_string(gourdNum));
}
void Player::useGourd() {
    if (gourdNum > 0) {
        Effect::healing(world, scene, sprite->getPosition());
        updateGourd(-1);
        healing(1);
    }
    else {
        Effect::soundError();
    }
}

// Hàm để lưu thông tin nhân vật vào file
void Player::savePlayerDataInit() {
    std::ofstream outFile("atrInit.txt");
    if (outFile.is_open()) {

        outFile << maxHealth << "\n"; 
        outFile << maxMana << "\n"; 
        outFile << maxStickNum << "\n";
        outFile << gem << "\n"; 
        outFile << health << "\n";
        outFile << mana << "\n";
        outFile << stickNum << "\n";
        outFile << gourdNum << "\n";
        outFile.close();
        std::cout << "Data saved successfully." << std::endl;
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void Player::loadPlayerDataInit(bool isNew) {
    std::ifstream inFile("atrInit.txt");
    if (inFile.is_open()) {
        inFile >> maxHealth; 
        inFile >> maxMana; 
        inFile >> maxStickNum; 
        inFile >> gem;
        inFile >> health;
        inFile >> mana;
        inFile >> stickNum;
        inFile >> gourdNum;
        inFile.close();
        healthVector.clear();
        uiNode->removeAllChildrenWithCleanup(true);
        if (isNew) {
            health = maxHealth;
            stickNum = maxStickNum;
            mana = maxMana;
        }
        
        initItem();
        initHealth();
    }
}


void Player::hurt() {
    if (!isAlive) return;
    Effect::playerHurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-GetHit_", 13, 0.02));
    animate->setTag(4);
    sprite->runAction(animate);
}

void Player::die() {
    savePlayerDataInit();
    isAlive = false;
    Effect::playerHurt();
    body->SetLinearVelocity(b2Vec2_zero);
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-Death_", 25, 0.02));
    // Run animation with a callback
    auto callback = [this]() {
        auto sceneGameOver = GameOver::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, sceneGameOver));
    };
    auto callFunc = CallFunc::create(callback);
    auto sequence = Sequence::create(animate, callFunc, nullptr);
    sprite->runAction(sequence);
}

void Player::updateHealth(int damage) {
    health -= damage;
    updateHealthBar(health);
    hurt();
    if (health <= 0) {
        healthBar->removeFromParentAndCleanup(true);
        die();

    }
}

void Player::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / maxHealth;
    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(health/ 15* Common::scaleXSprite()); // Điều chỉnh chiều rộng thanh máu
    healthBarBg->setScaleX(maxHealth / 15* Common::scaleXSprite());
}
void Player::createHealthBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int y = 1075 * Common::scaleYSprite();
    int x = origin.x+140 * Common::scaleSprite();

    // Tạo nền thanh máu
    healthBarBg = Sprite::create("ui/health_bar_bg3.png");
    healthBarBg->setPosition(Vec2(x,y)); // Đặt vị trí của nền thanh máu
    uiNode->addChild(healthBarBg);
    healthBarBg->setAnchorPoint(Vec2(0, 1));
    healthBarBg->setScale(Common::HEALTH_BAR_SCALE* Common::scaleSprite());
    healthBarBg->setScaleX(maxHealth / 15* Common::scaleXSprite());

    // Tạo thanh máu
    healthBar = Sprite::create("ui/health_bar3.png");
    healthBar->setPosition(healthBarBg->getPosition()); // Đặt cùng vị trí với nền
    uiNode->addChild(healthBar);
    healthBar->setAnchorPoint(Vec2(0, 1));
    healthBar->setScale(Common::HEALTH_BAR_SCALE* Common::scaleSprite());
    healthBar->setScaleX(maxHealth/15* Common::scaleXSprite());
    updateHealthBar(health);
}

// Mana
void Player::createManaBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int y = 1040 * Common::scaleYSprite();
    int x = origin.x+140 * Common::scaleSprite();
    // Tạo nền thanh máu
    manaBarBg = Sprite::create("ui/health_bar_bg3.png");
    manaBarBg->setPosition(Vec2(x, y)); // Đặt vị trí của nền thanh máu
    uiNode->addChild(manaBarBg);
    manaBarBg->setAnchorPoint(Vec2(0, 1));
    manaBarBg->setScale(Common::HEALTH_BAR_SCALE* Common::scaleSprite());
    manaBarBg->setScaleX(maxMana / 15* Common::scaleXSprite());

    // Tạo thanh máu
    manaBar = Sprite::create("ui/mana_bar.png");
    manaBar->setPosition(manaBarBg->getPosition()); // Đặt cùng vị trí với nền
    uiNode->addChild(manaBar);
    manaBar->setAnchorPoint(Vec2(0, 1));
    manaBar->setScale(Common::HEALTH_BAR_SCALE * Common::scaleSprite());
    manaBar->setScaleX(maxMana/15 * Common::scaleXSprite());
    updateManaBar(mana);
}

void Player::addMana(int num) {
    mana+=num;
    mana = mana > maxMana ? maxMana : mana;
    updateManaBar(mana);
}


void Player::useMana(int num) {
    mana-= num;
    mana = mana <= 0 ? 0 : mana;
    updateManaBar(mana);
}

void Player::updateManaBar(float mana) {
    // Tính toán tỷ lệ máu hiện tại
    float manaRatio = mana / maxMana;
    // Cập nhật kích thước của thanh máu
    manaBar->setScaleX(mana / 15 *Common::scaleXSprite()); // Điều chỉnh chiều rộng thanh máu
    manaBarBg->setScaleX(maxMana / 15 * Common::scaleXSprite()); // Điều chỉnh chiều rộng thanh máu
}

// Getter cho maxMana
float Player::getMaxMana() const {
    return maxMana;
}

// Setter cho maxMana
void Player::setMaxMana(float newMaxMana) {
    if (newMaxMana >= 0) { // Kiểm tra để đảm bảo maxMana không âm
        maxMana = newMaxMana;
    }
}

// Getter cho mana
float Player::getMana() const {
    return mana;
}

// Setter cho mana
void Player::setMana(float newMana) {
    if (newMana >= 0 && newMana <= maxMana) { // Kiểm tra để đảm bảo mana hợp lệ
        mana = newMana;
    }
}

// Set maxHealth
void Player::setMaxHealth(float health) {
    maxHealth = health;
}

// Get maxHealth
float Player::getMaxHealth() const {
    return maxHealth;
}

void Player::updateMove() {
    if (!isInVillage  && scene->getChildByName("Menu") == nullptr) {
        isEnable = true;
    }
    if (!isEnable||!isAlive) return;
    if (keys[EventKeyboard::KeyCode::KEY_A]) {
        body->SetLinearVelocity(b2Vec2(-Common::SPEED_PLAYER *Common::scaleSprite(), body->GetLinearVelocity().y)); // Di chuyển sang trái
        sprite->setScaleX(-Common::PLAYER_SCALE * Common::scaleSprite());
        if (!sprite->getActionByTag(4) && !sprite->getActionByTag(1)) { // Kiểm tra nếu hoạt ảnh chưa chạy
            walk();
        }
    }
    else if (keys[EventKeyboard::KeyCode::KEY_D]) {
        body->SetLinearVelocity(b2Vec2(Common::SPEED_PLAYER * Common::scaleSprite(), body->GetLinearVelocity().y)); // Di chuyển sang phải
        sprite->setScaleX(Common::PLAYER_SCALE * Common::scaleSprite());
        if (!sprite->getActionByTag(4) && !sprite->getActionByTag(1)) { // Kiểm tra nếu hoạt ảnh chưa chạy
            walk();
        }
    }
    else {
        // Dừng lại nếu không nhấn phím nào
        body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
        if (!sprite->getActionByTag(4) && !sprite->getActionByTag(2)) { // Kiểm tra nếu hoạt ảnh chưa chạy
            idle();
        }
    }
}

void Player::actionKey(EventKeyboard::KeyCode keyCode) {
    if (isEnable) {
        if (keyCode == (EventKeyboard::KeyCode::KEY_SPACE)) {
            if (body->GetContactList() != nullptr) { // Kiểm tra xem có tiếp xúc với mặt đất không
                jump();
            }
        }
        if (!isInVillage) {
            if (keyCode == (EventKeyboard::KeyCode::KEY_E)) {
                throwStick();
            }

            if (keyCode == (EventKeyboard::KeyCode::KEY_R)) {
                useGourd();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_Q)) {
                eagle();
            }
        } 
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_ESCAPE)) {
        Common::togglePause(isEnable, scene);
    }
}

void Player::updateSlashVector(float dt) {
    // Cập nhật tất cả các slash
    for (auto it = slashVector.begin(); it != slashVector.end(); ) {
        (*it)->update(dt, scene);

        if (!(*it)->IsVisible()) {
            // Xóa sprite khỏi danh sách và bộ nhớ
            delete* it;
            it = slashVector.erase(it); // Loại bỏ sprite khỏi danh sách
        }
        else {
            ++it;
        }
    }
}

// Mouse event
void Player::initMouseEvent() {
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event* event) {
        if (isEnable && !isInVillage) {
            Slash* slash = hit();
            if (slash != nullptr)
                slashVector.push_back(slash);
        }
        };
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, scene);
}

// Key event
void Player::initKeyEvent() {
    // Hold key
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        keys[keyCode] = true;
        actionKey(keyCode);
        };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        keys[keyCode] = false;
        };
    scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, scene);
}