#include "UpgradeShopLayer.h"
#include "main/Effect.h"


UpgradeShopLayer* UpgradeShopLayer::createLayer(Player* player, Scene* scene)
{
    UpgradeShopLayer* ret = new UpgradeShopLayer();
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
bool UpgradeShopLayer::init(Player* player, Scene* scene) {

    this->player = player;
    player->isEnable = false;
    player->getBody()->SetLinearVelocity(b2Vec2_zero);
    player->idle();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300 * Common::scaleSizeX(), 400 * Common::scaleSizeY());
    layer->setPosition(Vec2(origin.x+screenSize.width/2.5, screenSize.height/3)); // Vị trí khung trên bản đồ

     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Common::scalePos(150,200)); // Giữa khung
    background->setScaleX(0.8* Common::scaleSizeXY());
    background->setScaleY(1 * Common::scaleSizeY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_upgrade.png");
    header->setPosition(Common::scalePos(150, 435));
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);

    int skillPostX = 30 * Common::scaleSizeX();
    int pricePostX = 90 * Common::scaleSizeX();

    // Health Upgrade
    Sprite* healthUpgrade = Sprite::create("Item/gourd/heart.png");
    healthUpgrade->setPosition(skillPostX, 340 * Common::scaleSizeY());
    healthUpgrade->setScale(0.2 * Common::scaleSizeXY());

    layer->addChild(healthUpgrade, 10);
    auto healthUpgradeLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max health", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    healthUpgradeLabel->setColor(cocos2d::Color3B(0, 0, 0));
    healthUpgradeLabel->setPosition(pricePostX, 360 * Common::scaleSizeY());
    healthUpgradeLabel->setAnchorPoint(Vec2(0, 0));
    layer->addChild(healthUpgradeLabel);


    auto healthPrice = MenuItemImage::create(
        "shop/200.png",   // image when button is off
        "shop/200.png",   // image when button is pressed
        CC_CALLBACK_1(UpgradeShopLayer::upgradeHealth, this)
    );
    healthPrice->setScale(0.6 * Common::scaleSizeXY());
    healthPrice->setTag(200); // Đặt tag cho vật phẩm
    healthPrice->setAnchorPoint(Vec2(0, 0.5));
    healthPrice->setPosition(Vec2(pricePostX, 320 * Common::scaleSizeY()));


    // Mana upgrade ---------------------------------------------
    Sprite* manaUpgrade = Sprite::create("shop/mana.png");
    manaUpgrade->setPosition(skillPostX, 210 * Common::scaleSizeXY());
    manaUpgrade->setScale(0.2 * Common::scaleSizeXY());

    layer->addChild(manaUpgrade, 10);
    auto manaUpgradeLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max mana", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    manaUpgradeLabel->setColor(cocos2d::Color3B(0, 0, 0));
    manaUpgradeLabel->setPosition(pricePostX, 230 *Common::scaleSizeY());
    manaUpgradeLabel->setAnchorPoint(Vec2(0, 0));
    layer->addChild(manaUpgradeLabel);

    auto manaUpgradeLabelPrice = MenuItemImage::create(
        "shop/200.png",   // image when button is off
        "shop/200.png",   // image when button is pressed
        CC_CALLBACK_1(UpgradeShopLayer::upgradeMana, this)
    );
    manaUpgradeLabelPrice->setScale(0.6 * Common::scaleSizeXY());
    manaUpgradeLabelPrice->setTag(200); // Đặt tag cho vật phẩm
    manaUpgradeLabelPrice->setAnchorPoint(Vec2(0, 0.5));
    manaUpgradeLabelPrice->setPosition(Vec2(pricePostX, 190 * Common::scaleSizeY()));


    // stick upgrade
    Sprite* stickUpgrade = Sprite::create("ui/BackStick.png");
    stickUpgrade->setPosition(skillPostX, 90 * Common::scaleSizeY());
    stickUpgrade->setScale(0.25 * Common::scaleSizeXY());
    layer->addChild(stickUpgrade, 10);

    auto stickUpgradeLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    stickUpgradeLabel->setColor(cocos2d::Color3B(0, 0, 0));
    stickUpgradeLabel->setPosition(pricePostX, 110 * Common::scaleSizeY());
    stickUpgradeLabel->setAnchorPoint(Vec2(0, 0));
    layer->addChild(stickUpgradeLabel);
    auto stickPrice = MenuItemImage::create(
        "shop/200.png",   // image when button is off
        "shop/200.png",   // image when button is pressed
        CC_CALLBACK_1(UpgradeShopLayer::upgradeStick, this)
    );
    stickPrice->setScale(0.6 * Common::scaleSizeXY());
    stickPrice->setTag(200); // Đặt tag cho vật phẩm
    stickPrice->setAnchorPoint(Vec2(0, 0.5));
    stickPrice->setPosition(Vec2(pricePostX, 70 * Common::scaleSizeY())); // Vị trí trong khung


    auto menuItem = Menu::create(healthPrice, manaUpgradeLabelPrice, stickPrice, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    layer->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto closeItem = MenuItemLabel::create(
        Label::createWithSystemFont("Cancel", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(UpgradeShopLayer::menuCloseCallback, this)
    );
    closeItem->setPosition(Vec2(80*Common::scaleSizeX(), -10 * Common::scaleSizeY()));
    closeItem->setColor(cocos2d::Color3B(180, 57, 67));
    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Accept", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(UpgradeShopLayer::menuOKCallback, this)
    );
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    okItem->setPosition(Vec2(200 * Common::scaleSizeX(), -10 * Common::scaleSizeY()));

    auto menu = Menu::create(closeItem, okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    layer->addChild(menu); // Thêm menu vào khung

    this->setName("popup");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void UpgradeShopLayer::upgradeStick(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemImage*>(sender);
    Common::zoomAction(item, 0.1, 1.1);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->getGem() >= price) {
        player->setMaxStickNum(player->getMaxStickNum() + 1);
        player->updateStickNum(1);
        player->updateGem(-price);
        
    }
    else MusicManager::getInstance()->soundError();
}

void UpgradeShopLayer::upgradeHealth(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemImage*>(sender);
    Common::zoomAction(item, 0.1, 1.1);

    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->getGem() >= price) {
        player->setMaxHealth(player->getMaxHealth()+1);
        player->healing(1);
        player->updateGem(-price);
    }
    else MusicManager::getInstance()->soundError();
}

void UpgradeShopLayer::menuCloseCallback(cocos2d::Ref* Sender) {
    player->loadPlayerDataInit(true);
    this->removeFromParentAndCleanup(true);
    
}

void UpgradeShopLayer::menuOKCallback(cocos2d::Ref* Sender) {
    player->savePlayerDataInit();
    this->removeFromParentAndCleanup(true);
}
void UpgradeShopLayer::upgradeMana(cocos2d::Ref* Sender) {
    auto item = static_cast<MenuItemImage*>(Sender);
    Common::zoomAction(item, 0.1, 1.1);

    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->getGem() >= price) {
        player->setMaxMana(player->getMaxMana() + 1);
        player->addMana(1);
        player->updateGem(-price);
    }
    else MusicManager::getInstance()->soundError();
}
