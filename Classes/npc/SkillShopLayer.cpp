#include "SkillShopLayer.h"
#include "main/Effect.h"
#include "scene/MenuScene.h"

USING_NS_CC;

SkillShopLayer::SkillShopLayer() {
    //if (!Layer::init()) {
    //    return false; // Kiểm tra khởi tạo
    //}
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    shopFrame = LayerColor::create(Color4B(0, 0, 0, 0), 300, 400);
    shopFrame->setPosition(Vec2(origin.x+screenSize.width / 2, screenSize.height / 3));
    //this->addChild(shopFrame);

    
     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Vec2(150, 200) * Common::scaleSizeXY()); // Giữa khung
    background->setScale(0.8 * Common::scaleSizeXY());
    shopFrame->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_shop.png");
    header->setPosition(Vec2(150, 370) * Common::scaleSizeXY());
    header->setScale(Common::scaleSizeXY());
    shopFrame->addChild(header);

    int skillPostX = 50 * Common::scaleSizeX();
    int pricePostX = 100 * Common::scaleSizeX();

    // Skill 1
    Sprite* skill1 = Sprite::create("player/skill/YellowEyes.png");
    skill1->setPosition(skillPostX, 300 * Common::scaleSizeY());
    skill1->setScale(0.15 * Common::scaleSizeXY());
    shopFrame->addChild(skill1, 10);
    auto skill1Label = MenuItemLabel::create(
        Label::createWithSystemFont("Fiery yellow eyes", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    skill1Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill1Label->setPosition(pricePostX, 300*Common::scaleSizeY());
    skill1Label->setAnchorPoint(Vec2(0,0));
    shopFrame->addChild(skill1Label);
    
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("50000 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(SkillShopLayer::purchaseEyesSkill, this)
    );
    itemLabel1->setColor(cocos2d::Color3B(255, 255, 0));
    Common::zoomAction(itemLabel1);
    itemLabel1->setTag(50000); // Đặt tag cho vật phẩm
    itemLabel1->setAnchorPoint(Vec2(0, 0));
    itemLabel1->setPosition(Vec2(pricePostX, 260 * Common::scaleSizeY()));
    
    // ----------------------------------------skill 2
    Sprite* skill2 = Sprite::create("Item/gourd/gourd.png");
    skill2->setPosition(skillPostX, 180* Common::scaleSizeY());
    skill2->setScale(0.5 * Common::scaleSizeXY());
    shopFrame->addChild(skill2, 10);

    auto skill2Label = MenuItemLabel::create(
        Label::createWithSystemFont("Gourd", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    
    skill2Label->setColor(cocos2d::Color3B(0, 0, 0));
    skill2Label->setPosition(pricePostX, 180 * Common::scaleSizeY());
    skill2Label->setAnchorPoint(Vec2(0, 0));
    shopFrame->addChild(skill2Label);
    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("50 Gem", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(SkillShopLayer::purchaseGourd, this)
    );
    Common::zoomAction(itemLabel2);
    itemLabel2->setColor(cocos2d::Color3B(255, 255, 0));
    itemLabel2->setTag(50); // Đặt tag cho vật phẩm
    itemLabel2->setAnchorPoint(Vec2(0, 0));
    itemLabel2->setPosition(Vec2(pricePostX, 140 * Common::scaleSizeY())); // Vị trí trong khung

    auto menuItem = Menu::create(itemLabel1, itemLabel2, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    shopFrame->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto closeItem = MenuItemLabel::create(
        Label::createWithSystemFont("Cancel", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(SkillShopLayer::menuCloseCallback, this)
    );
    closeItem->setColor(cocos2d::Color3B(180, 57, 67));
    closeItem->setPosition(Vec2(80, 30) * Common::scaleSizeXY());
    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Accept", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(SkillShopLayer::menuOKCallback, this)
    );
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    okItem->setPosition(Vec2(200, 30) * Common::scaleSizeXY());

    auto menu = Menu::create(closeItem, okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    shopFrame->addChild(menu); // Thêm menu vào khung
    shopFrame->setName("shop");
}

void SkillShopLayer::purchaseGourd(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm

    if (player->gem >= price) {
        player->updateGourd(1);
        player->updateGem(-price);
    }
    else Effect::soundError();
}

void SkillShopLayer::purchaseEyesSkill(cocos2d::Ref* sender) {
    auto item = static_cast<MenuItemLabel*>(sender);
    int price = item->getTag(); // Lấy giá của vật phẩm
    if (player->gem >= price) {
        player->gem -= price;
        player->healing(1);
        player->updateGem(0);
    }
    else Effect::soundError();
}

void SkillShopLayer::menuCloseCallback(cocos2d::Ref* pSender) {
    player->loadPlayerDataInit(true);
    shopFrame->removeFromParentAndCleanup(true);
    
}

void SkillShopLayer::menuOKCallback(cocos2d::Ref* pSender) {
    player->savePlayerDataInit();
    shopFrame->removeFromParentAndCleanup(true);
}
