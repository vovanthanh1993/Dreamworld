#include "InventoryLayer.h"
#include "main/Effect.h"
#include "scene/MenuScene.h"

USING_NS_CC;

InventoryLayer* InventoryLayer::createLayer(Player* player, Scene* scene)
{
    InventoryLayer* ret = new InventoryLayer();
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
bool InventoryLayer::init(Player* player, Scene* scene)
{
    if (!BaseLayer::init(player, scene)) {
        return false;
    }
    this->player = player;
    player->isEnable = false;
    player->getBody()->SetLinearVelocity(b2Vec2_zero);
    player->idle();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();

    // Tạo khung nhỏ cho shop
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300* Common::scaleSizeX(), 400 * Common::scaleSizeY());

    //Thêm hình nền cho khung
    //auto background = Sprite::create("inventory/bg.png");
    auto background = Sprite::create("ui/shopbg.png");
    background->setPosition(Common::scalePos(150,200)); // Giữa khung
    background->setScale(1 * Common::scaleSizeXY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_charm.png");
    header->setPosition(Common::scalePos(150, 460));
    header->setScale(0.17*Common::scaleSizeXY());
    layer->addChild(header);

    int spaceY = 370 * Common::scaleSizeXY();
    for (const auto& charm : player->charmVector)
    {
        MenuItemImage* offItem = cocos2d::MenuItemImage::create(
            charm->spritePath,   // image when button is off
            charm->spritePath,   // image when button is pressed
            CC_CALLBACK_1(InventoryLayer::selectCharm, this)
        );
        
        offItem->setScale(0.3 * Common::scaleSizeXY());
        offItem->setPosition(-30* Common::scaleSizeXY(), spaceY-5* Common::scaleSizeXY());
        offItem->setUserData(charm);
        if (!charm->getIsActive()) {
           // offItem->setColor(cocos2d::Color3B::GREEN);
            offItem->setOpacity(100);
        }
        offItem->setTag(charm->id);
        // Tạo các mục shop
        auto itemLabel = MenuItemLabel::create(
            Label::createWithSystemFont(charm->getEffectString(), "fonts/Marker Felt.ttf", 20 * Common::scaleSizeXY())
        );
        itemLabel->setPosition(Common::scaleSizeXY(), spaceY);
        itemLabel->setAnchorPoint(Vec2(0,1));
        itemLabel->setColor(cocos2d::Color3B(0, 0, 0));
        
        layer->addChild(itemLabel);

        spaceY -= 70* Common::scaleSizeXY();
        Common::zoomAction(offItem);
        menuItems.pushBack(offItem);
    }
    
    //auto okItem = MenuItemLabel::create(
    //    Label::createWithSystemFont("OK", "fonts/Marker Felt.ttf", 20 * Common::scaleSizeXY()),
    //    CC_CALLBACK_1(InventoryLayer::menuOKCallback, this)
    //);
    //okItem->setColor(cocos2d::Color3B(180, 57, 67));
    //okItem->setPosition(Vec2(150 * Common::scaleSizeX(), -130 * Common::scaleSizeY()));
    //Menu* menuButton = Menu::create(okItem, nullptr);
    //menuButton->setPosition(Vec2::ZERO);  // Đặt vị trí menu gốc
    //layer->addChild(menuButton); // Thêm menu vào scene

    // Tạo menu và thêm các vật phẩm vào menu
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);  // Đặt vị trí menu gốc
    layer->addChild(menu); // Thêm menu vào scene

    this->setName("popup");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void InventoryLayer::selectCharm(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemImage*>(sender);
    Charm* charm = static_cast<Charm*>(item->getUserData());
    charm->setIsActive(true);
    //item->setColor(cocos2d::Color3B::GREEN);
    item->setOpacity(255);
    MusicManager::getInstance()->soundTing();
    player->changeCharm(charm);

    for (const auto& menuItem : menuItems) {
        if (item != menuItem) {
            //menuItem->setColor(cocos2d::Color3B::WHITE);
            menuItem->setOpacity(100);
            static_cast<Charm*>(static_cast<MenuItemImage*>(menuItem)->getUserData())->setIsActive(false);
        }
    }
}

void InventoryLayer::menuCloseCallback(Ref* Sender) {
    this->removeFromParentAndCleanup(true);
}

void InventoryLayer::menuOKCallback(Ref* Sender) {
    this->removeFromParentAndCleanup(true);
}
