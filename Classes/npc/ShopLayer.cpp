#include "ShopLayer.h"
#include "main/Effect.h"


ShopLayer* ShopLayer::createLayer(Player* player, Scene* scene)
{
    ShopLayer* ret = new ShopLayer();
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
bool ShopLayer::init(Player* player, Scene* scene) {

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
    background->setPosition(Vec2(150 * Common::scaleSizeX(), 200 * Common::scaleSizeY())); // Giữa khung
    background->setScale(0.8 * Common::scaleSizeXY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_upgrade.png");
    header->setPosition(Vec2(150 * Common::scaleSizeX(), 370 * Common::scaleSizeY()));
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);

    int skillPostX = 40 * Common::scaleSizeX();
    int pricePostX = 100 * Common::scaleSizeX();

    // Skill 1
    Sprite* skill1 = Sprite::create("Item/gourd/heart.png");
    skill1->setPosition(skillPostX, 300 * Common::scaleSizeY());
    skill1->setScale(0.15 * Common::scaleSizeXY());
    layer->addChild(skill1, 10);
    auto skill1Label = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max health", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    skill1Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill1Label->setPosition(pricePostX, 300 * Common::scaleSizeY());
    skill1Label->setAnchorPoint(Vec2(0, 0));
    layer->addChild(skill1Label);

    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("200 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ShopLayer::purchaseHealthItem, this)
    );
    itemLabel1->setColor(cocos2d::Color3B(255, 255, 0));
    itemLabel1->setTag(200); // Đặt tag cho vật phẩm
    itemLabel1->setAnchorPoint(Vec2(0, 0));
    itemLabel1->setPosition(Vec2(pricePostX, 260 * Common::scaleSizeY()));
    Common::zoomAction(itemLabel1);

    // update mana ---------------------------------------------
    Sprite* manaUpgrade = Sprite::create("shop/mana.png");
    manaUpgrade->setPosition(skillPostX, 200 * Common::scaleSizeXY());
    manaUpgrade->setScale(0.15 * Common::scaleSizeXY());
    layer->addChild(manaUpgrade, 10);
    auto manaUpgradeLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max mana", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    manaUpgradeLabel->setColor(cocos2d::Color3B(0, 0, 0));
    manaUpgradeLabel->setPosition(pricePostX, 200 *Common::scaleSizeY());
    manaUpgradeLabel->setAnchorPoint(Vec2(0, 0));
    layer->addChild(manaUpgradeLabel);

    auto manaUpgradeLabelPrice = MenuItemLabel::create(
        Label::createWithSystemFont("200 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ShopLayer::upgradeManaItem, this)
    );
    manaUpgradeLabelPrice->setColor(cocos2d::Color3B(255, 255, 0));
    manaUpgradeLabelPrice->setTag(200); // Đặt tag cho vật phẩm
    manaUpgradeLabelPrice->setAnchorPoint(Vec2(0, 0));
    manaUpgradeLabelPrice->setPosition(Vec2(pricePostX, 160 * Common::scaleSizeY()));
    Common::zoomAction(manaUpgradeLabelPrice);

    // ----------------------------------------skill 2
    Sprite* skill2 = Sprite::create("ui/BackStick.png");
    skill2->setPosition(skillPostX, 100 * Common::scaleSizeY());
    skill2->setScale(0.2 * Common::scaleSizeXY());
    layer->addChild(skill2, 10);

    auto skill2Label = MenuItemLabel::create(
        Label::createWithSystemFont("Upgrade max stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    skill2Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill2Label->setPosition(pricePostX, 100 * Common::scaleSizeY());
    skill2Label->setAnchorPoint(Vec2(0, 0));
    layer->addChild(skill2Label);
    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("200 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ShopLayer::purchaseStickItem, this)
    );
    itemLabel2->setColor(cocos2d::Color3B(255, 255, 0));
    itemLabel2->setTag(200); // Đặt tag cho vật phẩm
    itemLabel2->setAnchorPoint(Vec2(0, 0));
    itemLabel2->setPosition(Vec2(pricePostX, 60 * Common::scaleSizeY())); // Vị trí trong khung
    Common::zoomAction(itemLabel2);

    auto menuItem = Menu::create(itemLabel1, manaUpgradeLabelPrice, itemLabel2, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    layer->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto closeItem = MenuItemLabel::create(
        Label::createWithSystemFont("Cancel", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ShopLayer::menuCloseCallback, this)
    );
    closeItem->setPosition(Vec2(80*Common::scaleSizeX(), 30 * Common::scaleSizeY()));
    closeItem->setColor(cocos2d::Color3B(180, 57, 67));
    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Accept", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ShopLayer::menuOKCallback, this)
    );
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    okItem->setPosition(Vec2(200 * Common::scaleSizeX(), 30 * Common::scaleSizeY()));

    auto menu = Menu::create(closeItem, okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    layer->addChild(menu); // Thêm menu vào khung

    this->setName("shop");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void ShopLayer::purchaseStickItem(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->gem -= price;
        player->maxStickNum += 1;
        player->updateStickNum(1);
        player->updateGem(0);
        
    }
    else Effect::soundError();
}

void ShopLayer::purchaseHealthItem(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->setMaxHealth(player->getMaxHealth()+1);
        player->healing(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}

void ShopLayer::menuCloseCallback(cocos2d::Ref* Sender) {
    player->loadPlayerDataInit(true);
    this->removeFromParentAndCleanup(true);
    
}

void ShopLayer::menuOKCallback(cocos2d::Ref* Sender) {
    player->savePlayerDataInit();
    this->removeFromParentAndCleanup(true);
}
void ShopLayer::upgradeManaItem(cocos2d::Ref* Sender) {
    auto item = static_cast<MenuItemLabel*>(Sender);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->setMaxMana(player->getMaxMana() + 1);
        player->addMana(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}
