#include "GUILayer.h"
#include "main/Effect.h"

USING_NS_CC;

bool GUILayer::init() {
    if (!Layer::init()) {
        return false; // Kiểm tra khởi tạo
    }
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    LayerColor* shopFrame = LayerColor::create(Color4B(0, 0, 0, 0), 300*Common::scaleXSprite(), 400 * Common::scaleYSprite());
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    shopFrame->setPosition(origin+screenSize/3); // Vị trí khung trên bản đồ
    this->addChild(shopFrame);

    
     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Common::scalePos(Vec2(150, 200))); // Giữa khung
    background->setScale(0.8 * Common::scaleSprite());
    shopFrame->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_fight.png");
    header->setPosition(Vec2(150, 370) * Common::scaleSprite());
    header->setScale(Common::scaleSprite());
    shopFrame->addChild(header);
    
    // Tạo các mục shop
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("A D to Move", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("Space to Jump", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    auto itemLabel3 = MenuItemLabel::create(
        Label::createWithSystemFont("E to throw Stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    auto itemLabel4 = MenuItemLabel::create(
        Label::createWithSystemFont("R to use Gourd", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    auto itemLabel5 = MenuItemLabel::create(
        Label::createWithSystemFont("1 2 3 4 to use Skill", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    auto itemLabel6 = MenuItemLabel::create(
        Label::createWithSystemFont("Left mouse to Slash", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    

    // Vị trí các mục nằm trong khung
    itemLabel1->setPosition(Vec2(150, 350) * Common::scaleSprite()); // Vị trí trong khung
    itemLabel1->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel2->setPosition(Vec2(150, 300) * Common::scaleSprite()); // Vị trí trong khung
    itemLabel2->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel3->setPosition(Vec2(150, 250) * Common::scaleSprite()); // Vị trí trong khung
    itemLabel3->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel4->setPosition(Vec2(150, 200) * Common::scaleSprite()); // Vị trí trong 
    itemLabel4->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel5->setPosition(Vec2(150, 150) * Common::scaleSprite()); // Vị trí trong khung
    itemLabel5->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel6->setPosition(Vec2(150, 100) * Common::scaleSprite()); // Vị trí trong khung
    itemLabel6->setColor(cocos2d::Color3B(0, 0, 0));

    auto menuItem = Menu::create(itemLabel1, itemLabel2, itemLabel3, itemLabel4, itemLabel5, itemLabel6, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    shopFrame->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Ok", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite()),
        CC_CALLBACK_1(GUILayer::menuOKCallback, this)
    );
    okItem->setPosition(Vec2(150, 30) * Common::scaleSprite());
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    auto menu = Menu::create(okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    shopFrame->addChild(menu); // Thêm menu vào khung
    shopFrame->setName("SHOP");

    return true; // Khởi tạo thành công
}

void GUILayer::menuOKCallback(cocos2d::Ref* pSender) {
    isEnable = true;
    this->removeFromParentAndCleanup(true);
    
}
