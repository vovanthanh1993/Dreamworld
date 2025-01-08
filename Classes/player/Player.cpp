#include "player/Player.h"
#include "main/Effect.h"
#include "player/skill/Stick.h"
#include <player/skill/Eagle.h>
#include "scene/GameOver.h"
#include <iostream>
#include <fstream>
#include "layer/PlayerStatsLayer.h"
#include "player/PlayerUI.h"

PlayerUI* playerUI;
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
    
    //scene->addChild(uiNode, 10);
    idle();

    initKeyEvent();
    initMouseEvent();

    // Su dung pool
    slashPool->createPool(world, scene, bodyToSpriteMap, 5);
    stickPool->createPool(world, scene, bodyToSpriteMap, 5);
    eaglePool->createPool(world, scene, bodyToSpriteMap, 10);

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
    MusicManager::getInstance()->playerJump();
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
    int manaUse = 7;
    if (!isAlive || mana < manaUse) return;
    float currentTime = Director::getInstance()->getTotalFrames() / 60.0f;

    // Kiểm tra nếu đang không tấn công hoặc đã qua thời gian chờ
    if ((currentTime - lastAttackTimeEagle >= attackCooldownEagle)) {
        lastAttackTimeEagle = currentTime;
        useMana(manaUse);
        MusicManager::getInstance()->eagle();
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

void Player::getDamage(int damage) {
    if (!isAlive) return;
    updateHealth(damage);
}
void Player::healing(int num) {
    healing();
    MusicManager::getInstance()->soundHealth();
    health += num;
    health = health > maxHealth ? maxHealth : health;
    playerUI->updateHealthBar(health);
}


void Player::updateStickNum(int stick) {
    stickNum += stick;
    stickNum = stickNum < 0 ? 0 : stickNum;
    stickNum = stickNum >= maxStickNum ? maxStickNum : stickNum;
    playerUI->stickLabel->setString("x" + to_string(stickNum));
    if (stick > 0) {
        MusicManager::getInstance()->getStick();
    }
}

void Player::updateGem(int i) {
    gem+=i;
    playerUI->gemLabel->setString("x" + to_string(gem));
    MusicManager::getInstance()->soundTing();
}

void Player::addHealthPotion(int i) {
    healthPotionNum += i;
    playerUI->healthPotionLabel->setString("x" + to_string(healthPotionNum));
}
void Player::useHealthPotion() {
    if (healthPotionNum > 0) {
        Effect::healing(world, scene, sprite->getPosition());
        addHealthPotion(-1);
        healing(2);
    }
    else {
        MusicManager::getInstance()->soundError();
    }
}

void Player::addManaPotion(int i) {
    manaPotionNum += i;
    playerUI->manaPotionLabel->setString("x" + to_string(manaPotionNum));
}
void Player::useManaPotion() {
    if (manaPotionNum > 0) {
        Effect::healing(world, scene, sprite->getPosition());
        MusicManager::getInstance()->soundHealth();
        addManaPotion(-1);
        addMana(2);
    }
    else {
        MusicManager::getInstance()->soundError();
    }
}

// Hàm để lưu thông tin nhân vật vào file
void Player::savePlayerDataInit() {
    std::ofstream outFile("atrInit.txt");
    if (outFile.is_open()) {

        outFile << maxHealth << "\n"; 
        outFile << maxMana << "\n"; 
        outFile << maxStickNum << "\n";
        outFile << health << "\n";
        outFile << mana << "\n";
        outFile << stickNum << "\n";
        outFile << healthPotionNum << "\n";
        outFile << manaPotionNum << "\n";
        outFile << gem << "\n";
        outFile << slashDamage << "\n";
        outFile << stickDamage << "\n";
        outFile << eagleDamage << "\n";
        //outFile << bossmap4 << "\n";
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
        inFile >> health;
        inFile >> mana;
        inFile >> stickNum;
        inFile >> healthPotionNum;
        inFile >> manaPotionNum;
        inFile >> gem;
        inFile >> slashDamage;
        inFile >> stickDamage;
        inFile >> eagleDamage;
        //inFile >> bossmap4;
        inFile.close();
        //uiNode->removeAllChildrenWithCleanup(true);
        
        if (isNew) {
            health = maxHealth;
            stickNum = maxStickNum;
            mana = maxMana;
            slashDamage = 10;
            stickDamage = 10;
            eagleDamage = 10;
            //bossmap4 = 0;
        }
       if(playerUI != nullptr)
            scene->removeChild(playerUI);
        playerUI = PlayerUI::createLayer(this, scene);

        if (!isNew)
            readCharmFromFile();
    }
}

void Player::hurt() {
    if (!isAlive) return;
    MusicManager::getInstance()->playerHurt();
    sprite->stopAllActions();
    auto animate = Animate::create(Common::createAnimation("Wukong-GetHit_", 13, 0.02));
    animate->setTag(4);
    sprite->runAction(animate);
}

void Player::die() {
    savePlayerDataInit();
    isAlive = false;
    MusicManager::getInstance()->playerHurt();
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
    if (!isAlive) return;

    health -= damage;
    playerUI->updateHealthBar(health);
    if(damage >0) hurt();
    
    if (health <= 0) {
        playerUI->healthBar->removeFromParentAndCleanup(true);
        die();
        return;
    }
}

void Player::addMana(int num) {
    mana+=num;
    mana = mana > maxMana ? maxMana : mana;
    playerUI->updateManaBar(mana);
}


void Player::useMana(int num) {
    mana-= num;
    mana = mana <= 0 ? 0 : mana;
    playerUI->updateManaBar(mana);
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

    if (!scene->getChildByName("popup") && !scene->getChildByName("shop")) {
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
        if (keyCode == (EventKeyboard::KeyCode::KEY_V)) {
            auto camera = scene->getDefaultCamera();
            auto playerStatsLayer = PlayerStatsLayer::createLayer(this, scene);
            Vec2 pos = camera->getPosition();
            playerStatsLayer->setPosition(Vec2(pos.x - 250 * Common::scaleSizeXY(), pos.y - 250 * Common::scaleSizeXY()));
        }

        if (!isInVillage) {
            if (keyCode == (EventKeyboard::KeyCode::KEY_O)) {
                throwStick();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_1)) {
                useHealthPotion();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_2)) {
                useManaPotion();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_P)) {
                throwEagle();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_I)) {
                hit();
            }
            if (keyCode == (EventKeyboard::KeyCode::KEY_C)) {
                auto camera = scene->getDefaultCamera();
                auto inventoryLayer = InventoryLayer::createLayer(this, scene);
                Vec2 pos = camera->getPosition();
                inventoryLayer->setPosition(Vec2(pos.x - 250 * Common::scaleSizeXY(), pos.y - 250 * Common::scaleSizeXY()));
            }
        } 
    }
    
    if (keyCode == (EventKeyboard::KeyCode::KEY_ESCAPE)) {
        if(1 == Common::togglePause(isEnable, scene)) savePlayerDataInit();
    }

    // Cheat code
    if (keyCode == (EventKeyboard::KeyCode::KEY_F1)) {
        auto villageScene = VillageScene::createScene("map/bglv1.png", "sound/bg1.mp3", "village", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F2)) {
        auto villageScene = Map1Scene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", true);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F3)) {
        auto villageScene = Boss1Scene::createScene("map/bglv1.png", "Enemy/Bossmap1/sound/bg.mp3", "boss1", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F4)) {
        auto villageScene = Map2Scene::createScene("map/bglv1.png", "sound/bg22.mp3", "map2", true);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F5)) {
        auto villageScene = Boss2Scene::createScene("map/bglv1.png", "Enemy/Bossmap2/sound/bg.mp3", "boss2", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F6)) {
        auto villageScene = Map3Scene::createScene("map/bg2.png", "sound/bg3.mp3", "map3", true);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F7)) {
        auto villageScene = Boss3Scene::createScene("map/bg2.png", "Enemy/Bossmap3/sound/bg.mp3", "boss3", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F8)) {
        auto villageScene = MikoScene::createScene("map/bg2.png", "sound/bg3.mp3", "mikomap", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F9)) {
        auto villageScene = Boss4Scene::createScene("map/bg2.png", "enemy/Bossmap4/sound/bg.mp3", "boss4", false);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_F10)) {
        auto villageScene = MemoryScene::createScene("map/bglv1.png", "sound/endbg.mp3", "memory", true);
        Director::getInstance()->replaceScene(villageScene);
    }
    if (keyCode == (EventKeyboard::KeyCode::KEY_TAB)) {
        health = maxHealth;
        mana = maxMana;
        gem = 5000;
        stickNum = maxStickNum;
        updateStickNum(0);
        updateGem(0);
        updateHealth(0);
        addMana(0);
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

void Player::addCharm(Charm* charmInput) {
    MusicManager::getInstance()->soundTing();
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
        slashDamage -= currentCharm->slashDamageBonus;
        stickDamage -= currentCharm->stickDamageBonus;
        eagleDamage -= currentCharm->eagleDamageBonus;
    }
    currentCharm = charm;

    playerUI->setSpriteCharm(currentCharm);
    slashDamage += currentCharm->slashDamageBonus;
    stickDamage += currentCharm->stickDamageBonus;
    eagleDamage += currentCharm->eagleDamageBonus;
}

void Player::writeCharmToFile() {
    // Mở file để lưu thông tin
    ofstream outFile("charm.txt");
    if (outFile.is_open()) {
        // Ghi mỗi đối tượng vào file
        for (auto& charm : charmVector) {
            outFile << charm->id << "," << charm->slashDamageBonus << "," << charm->stickDamageBonus << "," << charm->eagleDamageBonus << "," << charm->getIsActive() << endl;
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
            charm->spritePath = "inventory/charm" + to_string(charm->id) + ".png";

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->slashDamageBonus = std::stoi(temp);

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->stickDamageBonus = std::stoi(temp);

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->eagleDamageBonus = std::stoi(temp);

            charm->getEffectString();

            std::getline(ss, temp, ',');  // Get damageBonus
            charm->setIsActive(false);
            if (temp == "1") {
                charm->setIsActive(true);
                currentCharm = charm;
                playerUI->setSpriteCharm(charm);
            }
            charmVector.pushBack(charm);
            // Thêm đối tượng vào vector
        }
        inFile.close();
    }
}

// Getter và Setter cho healthPotionNum
int Player::getHealthPotionNum() const {
    return healthPotionNum;
}

void Player::setHealthPotionNum(int num) {
    if (num >= 0) { // Kiểm tra để tránh số lượng âm
        healthPotionNum = num;
    }
}

// Getter và Setter cho manaPotionNum
int Player::getManaPotionNum() const {
    return manaPotionNum;
}

void Player::setManaPotionNum(int num) {
    if (num >= 0) { // Kiểm tra để tránh số lượng âm
        manaPotionNum = num;
    }
}

// Getters
int Player::getGem() const { return gem; }
int Player::getMaxStickNum() const { return maxStickNum; }
float Player::getSlashDamage() const { return slashDamage; }
float Player::getStickDamage() const { return stickDamage; }
float Player::getEagleDamage() const { return eagleDamage; }

// Setters
void Player::setGem(int value) { gem = value; }
void Player::setMaxStickNum(int value) { maxStickNum = value; }
void Player::setSlashDamage(float value) { slashDamage = value; }
void Player::setStickDamage(float value) { stickDamage = value; }
void Player::setEagleDamage(float value) { eagleDamage = value; }