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
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300, 400);
    layer->setPosition(Vec2(origin.x+screenSize.width/2.5, screenSize.height/3)); // Vị trí khung trên bản đồ

     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    //auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Vec2(150 * Common::scaleSizeX(), 200 * Common::scaleSizeY())); // Giữa khung
    background->setScaleX(0.8* Common::scaleSizeXY());
    background->setScaleY(1 * Common::scaleSizeY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_upgrade.png");
    header->setPosition(Vec2(150 * Common::scaleSizeX(), 435 * Common::scaleSizeY()));
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);

    int skillPostX = 40 * Common::scaleSizeX();
    int pricePostX = 100 * Common::scaleSizeX();

    // Skill 1
    Sprite* skill1 = Sprite::create("Item/gourd/heart.png");
    skill1->setPosition(skillPostX, 340 * Common::scaleSizeY());
    skill1->setScale(0.2 * Common::scaleSizeXY());
    layer->addChild(skill1, 10);
    auto skill1Label = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max health", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    skill1Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill1Label->setPosition(pricePostX, 360 * Common::scaleSizeY());
    skill1Label->setAnchorPoint(Vec2(0, 0));
    layer->addChild(skill1Label);

    /*auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("200 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(UpgradeShopLayer::purchaseHealthItem, this)
    );*/

    auto itemLabel1 = MenuItemImage::create(
        "shop/200.png",   // image when button is off
        "shop/200.png",   // image when button is pressed
        CC_CALLBACK_1(UpgradeShopLayer::upgradeHealth, this)
    );
    itemLabel1->setScale(0.6 * Common::scaleSizeXY());
    itemLabel1->setTag(200); // Đặt tag cho vật phẩm
    itemLabel1->setAnchorPoint(Vec2(0, 0.5));
    itemLabel1->setPosition(Vec2(pricePostX, 320 * Common::scaleSizeY()));
    Common::zoomAction(itemLabel1);

    // update mana ---------------------------------------------
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
        CC_CALLBACK_1(UpgradeShopLayer::upgradeHealth, this)
    );
    manaUpgradeLabelPrice->setScale(0.6 * Common::scaleSizeXY());
    //manaUpgradeLabelPrice->setColor(cocos2d::Color3B(255, 255, 0));
    manaUpgradeLabelPrice->setTag(200); // Đặt tag cho vật phẩm
    manaUpgradeLabelPrice->setAnchorPoint(Vec2(0, 0.5));
    manaUpgradeLabelPrice->setPosition(Vec2(pricePostX, 190 * Common::scaleSizeY()));
    Common::zoomAction(manaUpgradeLabelPrice);

    // ----------------------------------------skill 2
    Sprite* skill2 = Sprite::create("ui/BackStick.png");
    skill2->setPosition(skillPostX, 90 * Common::scaleSizeY());
    skill2->setScale(0.25 * Common::scaleSizeXY());
    layer->addChild(skill2, 10);

    auto skill2Label = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    skill2Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill2Label->setPosition(pricePostX, 110 * Common::scaleSizeY());
    skill2Label->setAnchorPoint(Vec2(0, 0));
    layer->addChild(skill2Label);
    auto itemLabel2 = MenuItemImage::create(
        "shop/200.png",   // image when button is off
        "shop/200.png",   // image when button is pressed
        CC_CALLBACK_1(UpgradeShopLayer::upgradeHealth, this)
    );
    itemLabel2->setScale(0.6 * Common::scaleSizeXY());
    //itemLabel2->setColor(cocos2d::Color3B(255, 255, 0));
    itemLabel2->setTag(200); // Đặt tag cho vật phẩm
    itemLabel2->setAnchorPoint(Vec2(0, 0.5));
    itemLabel2->setPosition(Vec2(pricePostX, 70 * Common::scaleSizeY())); // Vị trí trong khung
    Common::zoomAction(itemLabel2);

    auto menuItem = Menu::create(itemLabel1, manaUpgradeLabelPrice, itemLabel2, nullptr);
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

    this->setName("shop");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void UpgradeShopLayer::upgradeStick(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    item->setColor(cocos2d::Color3B(255, 255, 0));
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->gem -= price;
        player->maxStickNum += 1;
        player->updateStickNum(1);
        player->updateGem(0);
        
    }
    else Effect::soundError();
}

void UpgradeShopLayer::upgradeHealth(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    item->setColor(cocos2d::Color3B(255, 255, 0));

    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->setMaxHealth(player->getMaxHealth()+1);
        player->healing(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
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
    auto item = static_cast<MenuItemLabel*>(Sender);
    item->setColor(cocos2d::Color3B(255, 255, 0));

    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->setMaxMana(player->getMaxMana() + 1);
        player->addMana(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}
