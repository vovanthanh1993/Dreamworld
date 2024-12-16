#include "ItemShopLayer.h"
#include "main/Effect.h"
#include "scene/MenuScene.h"

ItemShopLayer* ItemShopLayer::createLayer(Player* player, Scene* scene)
{
    ItemShopLayer* ret = new ItemShopLayer();
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
bool ItemShopLayer::init(Player* player, Scene* scene) {
    this->player = player;
    player->isEnable = false;
    player->getBody()->SetLinearVelocity(b2Vec2_zero);
    player->idle();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300, 400);
    layer->setPosition(Vec2(origin.x+screenSize.width / 2, screenSize.height / 3));

    
     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Vec2(150, 200) * Common::scaleSizeXY()); // Giữa khung
    background->setScale(0.8 * Common::scaleSizeXY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_shop.png");
    header->setPosition(Vec2(150, 370) * Common::scaleSizeXY());
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);

    int itemPosX = 50 * Common::scaleSizeX();
    int pricePosX = 110 * Common::scaleSizeX();

    // Skill 1
    Sprite* healthPotionShop = Sprite::create("Item/potions/health.png");
    healthPotionShop->setPosition(itemPosX, 300 * Common::scaleSizeY());
    healthPotionShop->setScale(0.18 * Common::scaleSizeXY());
    layer->addChild(healthPotionShop, 10);
    auto healthPotionLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Health Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    healthPotionLabel->setColor(cocos2d::Color3B(0, 0, 0));
    healthPotionLabel->setPosition(pricePosX, 320*Common::scaleSizeY());
    healthPotionLabel->setAnchorPoint(Vec2(0,0));
    layer->addChild(healthPotionLabel);
    
    auto healthBuyButton = MenuItemImage::create(
        "shop/50.png",   // image when button is off
        "shop/50.png",   // image when button is pressed
        CC_CALLBACK_1(ItemShopLayer::purchaseHealthPotion, this)
    );

    Common::zoomAction(healthBuyButton);
    healthBuyButton->setScale(0.6 * Common::scaleSizeXY());
    healthBuyButton->setTag(50); // Đặt tag cho vật phẩm
    healthBuyButton->setAnchorPoint(Vec2(0, 0));
    healthBuyButton->setPosition(Vec2(pricePosX, 250 * Common::scaleSizeY()));
    
    // ----------------------------------------skill 2
    Sprite* manaPotionShop = Sprite::create("Item/potions/mana.png");
    manaPotionShop->setPosition(itemPosX, 160* Common::scaleSizeY());
    manaPotionShop->setScale(0.18 * Common::scaleSizeXY());
    layer->addChild(manaPotionShop, 10);

    auto manaPotionLabel = MenuItemLabel::create(
        Label::createWithSystemFont("Mana Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    
    manaPotionLabel->setColor(cocos2d::Color3B(0, 0, 0));
    manaPotionLabel->setPosition(pricePosX, 180 * Common::scaleSizeY());
    manaPotionLabel->setAnchorPoint(Vec2(0, 0));
    layer->addChild(manaPotionLabel);

    auto manaBuyButton = MenuItemImage::create(
        "shop/50.png",   // image when button is off
        "shop/50.png",   // image when button is pressed
        CC_CALLBACK_1(ItemShopLayer::purchaseManaPotion, this)
    );
    Common::zoomAction(manaBuyButton);
    manaBuyButton->setScale(0.6 * Common::scaleSizeXY());
    manaBuyButton->setTag(50); // Đặt tag cho vật phẩm
    manaBuyButton->setAnchorPoint(Vec2(0, 0));
    manaBuyButton->setPosition(Vec2(pricePosX, 110 * Common::scaleSizeY())); // Vị trí trong khung

    auto menuItem = Menu::create(healthBuyButton, manaBuyButton, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    layer->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto closeItem = MenuItemLabel::create(
        Label::createWithSystemFont("Cancel", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ItemShopLayer::menuCloseCallback, this)
    );
    closeItem->setColor(cocos2d::Color3B(180, 57, 67));
    closeItem->setPosition(Vec2(80, 30) * Common::scaleSizeXY());
    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Accept", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(ItemShopLayer::menuOKCallback, this)
    );
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    okItem->setPosition(Vec2(200, 30) * Common::scaleSizeXY());

    auto menu = Menu::create(closeItem, okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    layer->addChild(menu); // Thêm menu vào khung

    this->setName("shop");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void ItemShopLayer::purchaseHealthPotion(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->addHealthPotion(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}

void ItemShopLayer::purchaseManaPotion(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm
    if (player->gem >= price) {
        player->addManaPotion(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}

void ItemShopLayer::menuCloseCallback(cocos2d::Ref* pSender) {
    player->loadPlayerDataInit(true);
    this->removeFromParentAndCleanup(true);
    
}

void ItemShopLayer::menuOKCallback(cocos2d::Ref* pSender) {
    player->savePlayerDataInit();
    this->removeFromParentAndCleanup(true);
}
