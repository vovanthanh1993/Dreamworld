#include "PlayerUI.h"

USING_NS_CC;
PlayerUI* PlayerUI::createLayer(Player* player, Scene* scene)
{
    PlayerUI* ret = new PlayerUI();
    if (ret && ret->init(player, scene)) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

//on "init" you need to initialize your instance
bool PlayerUI::init(Player* player, Scene* scene) {
    this->scene = scene;
    this->player = player;
    this->removeAllChildren();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    int y = screenSize.height;
    Sprite* head = Sprite::create("ui/head2.png");
    head->setPosition(origin.x, y);
    head->setAnchorPoint(Vec2(0, 1));
    head->setScale(0.8 * Common::scaleSizeXY());
    this->addChild(head);

    int xs = origin.x + 160 * Common::scaleSizeXY();
    //int xs = origin.x + head->getContentSize().width* head->getScale();
    Sprite* backStick = Sprite::create("ui/BackStick.png");
    backStick->setPosition(xs, y - 90 * Common::scaleSizeXY());
    backStick->setAnchorPoint(Vec2(0, 1));
    this->addChild(backStick);
    backStick->setScale(0.2 * Common::scaleSizeXY());

    xs += 50 * Common::scaleSizeXY();
    //stickNum = maxStickNum;
    stickLabel = Label::createWithTTF("x" + to_string(player->getStickNum()), "fonts/Marker Felt.ttf", 30);
    stickLabel->setPosition(xs, y - 130 * Common::scaleSizeXY());
    stickLabel->setScale(Common::scaleSizeXY());
    this->addChild(stickLabel);

    xs += 50 * Common::scaleSizeXY();
    Sprite* healthPotionSprite = Sprite::create("Item/potions/health.png");
    healthPotionSprite->setAnchorPoint(Vec2(0, 0.5));
    healthPotionSprite->setPosition(xs, y - 115 * Common::scaleSizeXY());
    healthPotionSprite->setScale(0.11 * Common::scaleSizeXY());
    this->addChild(healthPotionSprite);
    xs += 40 * Common::scaleSizeXY();
    healthPotionLabel = Label::createWithTTF("x" + to_string(player->getHealthPotionNum()), "fonts/Marker Felt.ttf", 30);
    healthPotionLabel->setAnchorPoint(Vec2(0, 0.5));
    healthPotionLabel->setPosition(xs, stickLabel->getPositionY());
    healthPotionLabel->setScale(Common::scaleSizeXY());
    this->addChild(healthPotionLabel);

    // mana
    xs += 60 * Common::scaleSizeXY();
    Sprite* manaPotionSprite = Sprite::create("Item/potions/mana.png");
    manaPotionSprite->setAnchorPoint(Vec2(0, 0.5));
    manaPotionSprite->setPosition(xs, y - 115 * Common::scaleSizeXY());
    manaPotionSprite->setScale(0.11 * Common::scaleSizeXY());
    this->addChild(manaPotionSprite);
    xs += 40 * Common::scaleSizeXY();
    manaPotionLabel = Label::createWithTTF("x" + to_string(player->getManaPotionNum()), "fonts/Marker Felt.ttf", 30);
    manaPotionLabel->setPosition(xs, stickLabel->getPositionY());
    manaPotionLabel->setAnchorPoint(Vec2(0, 0.5));
    manaPotionLabel->setScale(Common::scaleSizeXY());
    this->addChild(manaPotionLabel);

    xs += 60 * Common::scaleSizeXY();
    Sprite* gemSprite = Sprite::create("item/gem/gem.png");
    gemSprite->setPosition(xs, y - 100 * Common::scaleSizeXY());
    gemSprite->setAnchorPoint(Vec2(0, 1));
    this->addChild(gemSprite);
    gemSprite->setScale(0.1 * Common::scaleSizeXY());
    xs += 40 * Common::scaleSizeXY();
    gemLabel = Label::createWithTTF("x" + to_string(player->gem), "fonts/Marker Felt.ttf", 30);
    gemLabel->setPosition(xs, stickLabel->getPositionY());
    gemLabel->setAnchorPoint(Vec2(0, 0.5));
    gemLabel->setScale(Common::scaleSizeXY());
    this->addChild(gemLabel);
    scene->addChild(this, 10);
    this->schedule([this](float dt) { this->update(dt); }, "playerui");
    createHealthBar();
    createManaBar();
    return true;
}

void PlayerUI::createHealthBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int y = 1075 * Common::scaleSizeY();
    int x = origin.x + 140 * Common::scaleSizeXY();

    // Tạo nền thanh máu
    healthBarBg = Sprite::create("ui/health_bar_bg3.png");
    healthBarBg->setPosition(Vec2(x, y)); // Đặt vị trí của nền thanh máu
    this->addChild(healthBarBg);
    healthBarBg->setAnchorPoint(Vec2(0, 1));
    healthBarBg->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
    healthBarBg->setScaleX(player->getMaxHealth() / 15 * Common::scaleSizeX());

    // Tạo thanh máu
    healthBar = Sprite::create("ui/health_bar3.png");
    healthBar->setPosition(healthBarBg->getPosition()); // Đặt cùng vị trí với nền
    this->addChild(healthBar);
    healthBar->setAnchorPoint(Vec2(0, 1));
    healthBar->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
    healthBar->setScaleX(player->getMaxHealth() / 15 * Common::scaleSizeX());
    updateHealthBar(player->getHealth());
}

// Mana
void PlayerUI::createManaBar() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int y = 1040 * Common::scaleSizeY();
    int x = origin.x + 140 * Common::scaleSizeXY();
    // Tạo nền thanh máu
    manaBarBg = Sprite::create("ui/health_bar_bg3.png");
    manaBarBg->setPosition(Vec2(x, y)); // Đặt vị trí của nền thanh máu
    this->addChild(manaBarBg);
    manaBarBg->setAnchorPoint(Vec2(0, 1));
    manaBarBg->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
    manaBarBg->setScaleX(player->getMaxMana() / 15 * Common::scaleSizeX());

    // Tạo thanh máu
    manaBar = Sprite::create("ui/mana_bar.png");
    manaBar->setPosition(manaBarBg->getPosition()); // Đặt cùng vị trí với nền
    this->addChild(manaBar);
    manaBar->setAnchorPoint(Vec2(0, 1));
    manaBar->setScale(Constants::HEALTH_BAR_SCALE * Common::scaleSizeXY());
    manaBar->setScaleX(player->getMaxMana() / 15 * Common::scaleSizeX());
    updateManaBar(player->getMana());
}

void PlayerUI::updateHealthBar(float health) {
    // Tính toán tỷ lệ máu hiện tại
    float healthRatio = health / player->getMaxHealth();
    // Cập nhật kích thước của thanh máu
    healthBar->setScaleX(health / 15 * Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
    healthBarBg->setScaleX(player->getMaxHealth() / 15 * Common::scaleSizeX());
}
void PlayerUI::updateManaBar(float mana) {
    // Tính toán tỷ lệ máu hiện tại
    float manaRatio = mana / player->getMaxMana();
    // Cập nhật kích thước của thanh máu
    manaBar->setScaleX(mana / 15 * Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
    manaBarBg->setScaleX(player->getMaxMana() / 15 * Common::scaleSizeX()); // Điều chỉnh chiều rộng thanh máu
}

void PlayerUI::setSpriteCharm(Charm* charm, Sprite* charmSprite) {
    if (charmSprite != nullptr) {
        charmSprite->removeFromParentAndCleanup(true);
    }

    charmSprite = Sprite::create(charm->spritePath);
    charmSprite->setAnchorPoint(Vec2(0, 1));
    charmSprite->setScale(0.3 * Common::scaleSizeXY());

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    int y = screenSize.height;

    //Charm
    charmSprite->setPosition(origin.x + 50 * Common::scaleSizeXY(), y - 150 * Common::scaleSizeXY());
    this->addChild(charmSprite);
}

void PlayerUI::update(float dt) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto camera = scene->getDefaultCamera();
    int x = camera->getPositionX();
    int y = camera->getPositionY();
    Size screenSize = Director::getInstance()->getVisibleSize();
    this->setPositionX(camera->getPositionX() - screenSize.width / 2 - origin.x);
}

