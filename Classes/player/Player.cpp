#include "player/Player.h"
#include "main/Effect.h"
#include "player/skill/Stick.h"
#include <player/skill/Eagle.h>
#include "scene/GameOver.h"
#include <iostream>
#include <fstream>

Player::Player(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {
};
bool Player::init(bool isNew) {
    currentCharm = nullptr;

    spriteNode = SpriteBatchNode::create("player/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Wukong-Idle_0.png");
    sprite->setScale(Constants::PLAYER_SCALE* Common::scaleSizeXY());
    sprite->setPosition(position);
    sprite->setTag(Constants::TAG_PLAYER);
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
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_PLAYER;
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY|Constants::CATEGORY_LIMIT_MAP|Constants::CATEGORY_SLASH_ENEMY | Constants::CATEGORY_WALL| Constants::CATEGORY_ITEM| Constants::CATEGORY_CHEST| Constants::CATEGORY_ARROW | Constants::CATEGORY_STONE| Constants::CATEGORY_GEM; //  Không va chạm với CATEGORY_B
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    // load from file
    loadPlayerDataInit(isNew);
    scene->addChild(uiNode, 10);
    idle();

    initKeyEvent();
    initMouseEvent();

    // Su dung pool
    slashPool = new SlashPool(world, scene, 5);
    stickPool = new StickPool(world, scene, bodyToSpriteMap, 5);
    eaglePool = new EaglePool(world, scene, bodyToSpriteMap, 10);

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "player");
    scene->addChild(this);

    
    return true;
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
    b2Vec2 velocity(body->GetLinearVelocity().x, Constants::JUMP_PLAYER * Common::scaleSizeXY());
    body->SetLinearVelocity(velocity);
    auto repeatAnimate = RepeatForever::create(animate);
    repeatAnimate->setTag(3);
    sprite->runAction(repeatAnimate);
}

void Player::hit() {
    if (!isAlive) return;
    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
    // Kiểm tra nếu đang không tấn công hoặc đã qua thời gian chờ
    if ((currentTime - lastAttackTime >= attackCooldown)) {
        sprite->stopAllActions();
        lastAttackTime = currentTime; // Cập nhật thời gian của lần tấn công cuối
        auto animate = Animate::create(Common::createAnimation("Wukong-Hit_", 21, 0.013));
        
        // Run animation with a callback
        auto callback = [this]() {
            // Khi người chơi thực hiện đòn tấn công:
            Slash* slash = slashPool->getFromPool();
            if (slash != nullptr) {
                slash->init(Vec2(sprite->getPositionX() + direction * 60 * Common::scaleSizeXY(), sprite->getPositionY()));  // Kích hoạt nhát chém
                auto sprite = slash->getSprite();
                sprite->setScaleX(direction * slash->getScale() * Common::scaleSizeX());
            }
            
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sequence->setTag(4);
        sprite->runAction(sequence);
    }
}

void Player::throwStick() {
    if (!isAlive) return;
    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;
    // Kiểm tra nếu đang không tấn công hoặc đã qua thời gian chờ
    if ((currentTime - lastAttackTimeStick >= attackCooldownStick)) {
        lastAttackTimeStick = currentTime;
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("Wukong-Throw_", 20, 0.005));
        if (stickNum > 0) {
            Stick* stick = stickPool->getFromPool();
            if (stick != nullptr) {
                stick->init(Vec2(sprite->getPositionX() + direction * 10 * Common::scaleSizeXY(), sprite->getPositionY()));
                stick->getSprite()->setScaleX(direction * stick->getSprite()->getScale());
                b2Vec2 velocity(40 * direction * Common::scaleSizeXY(), 0);
                stick->getBody()->SetLinearVelocity(velocity);
                updateStickNum(-1);
            }
        }
        animate->setTag(4);
        sprite->runAction(animate);
    }
}


void Player::throwEagle() {
    int manaUse = 5;
    if (!isAlive || mana < manaUse) return;
    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;

    // Kiểm tra nếu đang không tấn công hoặc đã qua thời gian chờ
    if ((currentTime - lastAttackTimeEagle >= attackCooldownEagle)) {
        lastAttackTimeEagle = currentTime;
        useMana(manaUse);
        Effect::eagle();
        sprite->stopAllActions();
        auto animate = Animate::create(Common::createAnimation("Wukong-Throw_", 20, 0.005));

        int y = 0;
        int x = -100 * Common::scaleSizeX();
        for (int i = 1; i <= 3; i++) {
            Eagle* eagle = eaglePool->getFromPool();
            if (eagle != nullptr) {
                eagle->init(Vec2(sprite->getPositionX() + direction * x + direction * 15 * Common::scaleSizeXY(), sprite->getPositionY() + y));
                eagle->getSprite()->setScaleX(direction * eagle->getScale() * Common::scaleSizeX());
                b2Vec2 velocity(30 * direction * Common::scaleSizeXY(), 0);
                eagle->getBody()->SetLinearVelocity(velocity);
                y += 20 * Common::scaleSizeY();
                x += 100 * Common::scaleSizeX();
            }  
        }
        animate->setTag(4);
        sprite->runAction(animate);
    }
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
    head->setScale(0.8* Common::scaleSizeXY());
    uiNode->addChild(head);

    int xs = origin.x + 160* Common::scaleSizeXY();
    //int xs = origin.x + head->getContentSize().width* head->getScale();
    Sprite* backStick = Sprite::create("ui/BackStick.png");
    backStick->setPosition(xs, y - 90 * Common::scaleSizeXY());
    backStick->setAnchorPoint(Vec2(0, 1));
    uiNode->addChild(backStick);
    backStick->setScale(0.2* Common::scaleSizeXY());

    xs += 50 * Common::scaleSizeXY();
    //stickNum = maxStickNum;
    stickLabel = Label::createWithTTF("x" + to_string(stickNum), "fonts/Marker Felt.ttf", 30);
    stickLabel->setPosition(xs, y - 130 * Common::scaleSizeXY());
    stickLabel->setScale(Common::scaleSizeXY());
    uiNode->addChild(stickLabel);
    
    xs += 50* Common::scaleSizeXY();
    Sprite* gourd = Sprite::create("Item/gourd/gourd.png");
    gourd->setAnchorPoint(Vec2(0, 0.5));
    gourd->setPosition(xs, y - 115 * Common::scaleSizeXY());
    gourd->setScale(0.35* Common::scaleSizeXY());
    uiNode->addChild(gourd);
    xs += 60 * Common::scaleSizeXY();
    gourdLabel = Label::createWithTTF("x" + to_string(gourdNum), "fonts/Marker Felt.ttf", 30);
    gourdLabel->setPosition(xs, stickLabel->getPositionY());
    gourdLabel->setScale(Common::scaleSizeXY());
    uiNode->addChild(gourdLabel);

    xs += 40 * Common::scaleSizeXY();
    Sprite* gemSprite = Sprite::create("item/gem/gem.png");
    gemSprite->setPosition(xs, y - 100 * Common::scaleSizeXY());
    gemSprite->setAnchorPoint(Vec2(0, 1));
    uiNode->addChild(gemSprite);
    gemSprite->setScale(0.1* Common::scaleSizeXY());
    xs += 70 * Common::scaleSizeXY();
    gemLabel = Label::createWithTTF("x" + to_string(gem), "fonts/Marker Felt.ttf", 30);
    gemLabel->setPosition(xs, stickLabel->getPositionY());
    gemLabel->setScale(Common::scaleSizeXY());
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

    writeCharmToFile();
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
        readCharmFromFile();
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
    if(damage >0) hurt();
    
    if (health <= 0) {
        healthBar->removeFromParentAndCleanup(true);
        die();

    }
}

void Player::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / maxHealth;
    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(health/ 15* Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
    healthBarBg->setScaleX(maxHealth / 15* Common::scaleSizeX());
}
void Player::createHealthBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int y = 1075 * Common::scaleSizeY();
    int x = origin.x+140 * Common::scaleSizeXY();

    // Tạo nền thanh máu
    healthBarBg = Sprite::create("ui/health_bar_bg3.png");
    healthBarBg->setPosition(Vec2(x,y)); // Đặt vị trí của nền thanh máu
    uiNode->addChild(healthBarBg);
    healthBarBg->setAnchorPoint(Vec2(0, 1));
    healthBarBg->setScale(Constants::HEALTH_BAR_SCALE* Common::scaleSizeXY());
    healthBarBg->setScaleX(maxHealth / 15* Common::scaleSizeX());

    // Tạo thanh máu
    healthBar = Sprite::create("ui/health_bar3.png");
    healthBar->setPosition(healthBarBg->getPosition()); // Đặt cùng vị trí với nền
    uiNode->addChild(healthBar);
    healthBar->setAnchorPoint(Vec2(0, 1));
    healthBar->setScale(Constants::HEALTH_BAR_SCALE* Common::scaleSizeXY());
    healthBar->setScaleX(maxHealth/15* Common::scaleSizeX());
    updateHealthBar(health);
}

// Mana
void Player::createManaBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int y = 1040 * Common::scaleSizeY();
    int x = origin.x+140 * Common::scaleSizeXY();
    // Tạo nền thanh máu
    manaBarBg = Sprite::create("ui/health_bar_bg3.png");
    manaBarBg->setPosition(Vec2(x, y)); // Đặt vị trí của nền thanh máu
    uiNode->addChild(manaBarBg);
    manaBarBg->setAnchorPoint(Vec2(0, 1));
    manaBarBg->setScale(Constants::HEALTH_BAR_SCALE* Common::scaleSizeXY());
    manaBarBg->setScaleX(maxMana / 15* Common::scaleSizeX());

    // Tạo thanh máu
    manaBar = Sprite::create("ui/mana_bar.png");
    manaBar->setPosition(manaBarBg->getPosition()); // Đặt cùng vị trí với nền
    uiNode->addChild(manaBar);
    manaBar->setAnchorPoint(Vec2(0, 1));
    manaBar->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
    manaBar->setScaleX(maxMana/15 * Common::scaleSizeX());
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
    manaBar->setScaleX(mana / 15 * Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
    manaBarBg->setScaleX(maxMana / 15 * Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
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

void Player::update(float dt) {

    if (scene->getChildByName("shop") == nullptr) {
        isEnable = true;
    }
    if (!isEnable||!isAlive) return;

    if (keys[EventKeyboard::KeyCode::KEY_A]) {
        direction = -1;
        body->SetLinearVelocity(b2Vec2(-Constants::SPEED_PLAYER * Common::scaleSizeXY(), body->GetLinearVelocity().y)); // Di chuyển sang trái
        sprite->setScaleX(-Constants::PLAYER_SCALE * Common::scaleSizeXY());
        if (!sprite->getActionByTag(4) && !sprite->getActionByTag(1)) { // Kiểm tra nếu hoạt ảnh chưa chạy
            walk();
        }
    }
    else if (keys[EventKeyboard::KeyCode::KEY_D]) {
        direction = 1;
        body->SetLinearVelocity(b2Vec2(Constants::SPEED_PLAYER * Common::scaleSizeXY(), body->GetLinearVelocity().y)); // Di chuyển sang phải
        sprite->setScaleX(Constants::PLAYER_SCALE * Common::scaleSizeXY());
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
        if (keyCode == (EventKeyboard::KeyCode::KEY_SPACE) &&
            (Common::isCollision(body, Constants::TAG_WALL) || Common::isCollision(body, Constants::TAG_BRIDGE) ||
                Common::isCollision(body, Constants::TAG_BRIDGE_BREAK)|| Common::isCollision(body, Constants::TAG_BOX))) {
            jump();
        }
        if (!isInVillage) {
            if (keyCode == (EventKeyboard::KeyCode::KEY_E)) {
                throwStick();
            }

            if (keyCode == (EventKeyboard::KeyCode::KEY_R)) {
                useGourd();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_Q)) {
                throwEagle();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_I)) {
                auto camera = scene->getDefaultCamera();
                //Director::getInstance()->pause();
                auto inventoryLayer = InventoryLayer::createLayer(this, scene);
                Vec2 pos = camera->getPosition();
                inventoryLayer->setPosition(Vec2(pos.x -250*Common::scaleSizeXY(), pos.y - 250 * Common::scaleSizeXY()));
            }
        } 
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_ESCAPE)) {
        Common::togglePause(isEnable, scene);
    }
}

// Mouse event
void Player::initMouseEvent() {
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event* event) {
        if (isEnable && !isInVillage) {
            hit();
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

void Player::addEquipment(Charm* charmInput) {
    for (auto& charm : charmVector) {
        if (charmInput->id == charm->id) {
            if (currentCharm != nullptr && currentCharm->id == charm->id) {
                return;
            }
            charm = charmInput;
            return;
        }
    }
    charmVector.pushBack(charmInput);
}

void Player::changeCharm(Charm* charm) {
    
    if (currentCharm == charm) return;
    if (currentCharm) {
        maxHealth -= currentCharm->healthBonus;
        maxMana -= currentCharm->manaBonus;
        updateHealth(currentCharm->healthBonus);
        addMana(-currentCharm->manaBonus);
        damage -= currentCharm->damageBonus;
    }
    currentCharm = charm;

    if (charmSprite != nullptr) {
        charmSprite->removeFromParentAndCleanup(true);
    }
    
    charmSprite = Sprite::create(charm->spritePath);
    charmSprite->setAnchorPoint(Vec2(0,1));
    charmSprite->setScale(0.3 * Common::scaleSizeXY());
    //Common::zoomAction(charmSprite);

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    int y = screenSize.height;

    //Charm
    charmSprite->setPosition(origin.x, y - 150*Common::scaleSizeXY());
    uiNode->addChild(charmSprite);
    maxHealth += charm->healthBonus;
    maxMana += charm->manaBonus;
    updateHealth(-charm->healthBonus);
    addMana(charm->manaBonus);
    damage += charm->damageBonus;
}

void Player::writeCharmToFile() {
    // Mở file để lưu thông tin
    ofstream outFile("charm.txt");
    if (outFile.is_open()) {
        // Ghi mỗi đối tượng vào file
        for (auto& charm : charmVector) {
            outFile << charm->id << "," << charm->healthBonus << "," << charm->manaBonus << "," << charm->damageBonus << "," <<charm->getIsActive() << endl;
        }
        outFile.close();
    }
}

void Player::readCharmFromFile() {
    charmVector.clear();
    ifstream inFile("charm.txt");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            Charm* charm = new Charm(world, scene, bodyToSpriteMap);

            std::string temp;
            std::getline(ss, temp, ',');  // Get id
            charm->id = std::stoi(temp);   // Convert the string to an integer

            std::getline(ss, temp, ',');  // Get healthBonus
            charm->healthBonus = std::stoi(temp);

            std::getline(ss, temp, ',');  // Get manaBonus
            charm->manaBonus = std::stoi(temp);

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->damageBonus = std::stoi(temp);

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->setIsActive(false);
            if (temp == "1") {
                charm->setIsActive(true);
                changeCharm(charm);
            }


            charm->spritePath = "inventory/charm" + to_string(charm->id) + ".png";
            charm->getEffectString();
            charmVector.pushBack(charm);  // Thêm đối tượng vào vector
        }
        inFile.close();
    }
}