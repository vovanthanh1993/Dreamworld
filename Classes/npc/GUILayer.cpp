#include "GUILayer.h"

USING_NS_CC;
GUILayer* GUILayer::createLayer(Player* player, Scene* scene)
{
    GUILayer* ret = new GUILayer();
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
bool GUILayer::init(Player* player, Scene* scene) {

    this->player = player;
    player->isEnable = false;
    player->getBody()->SetLinearVelocity(b2Vec2_zero);
    player->idle();

    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300* Common::scaleSizeX(), 400 * Common::scaleSizeY());
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    layer->setPosition(origin+screenSize/3); // Vị trí khung trên bản đồ

     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Vec2(150, 200) * Common::scaleSizeXY()); // Giữa khung
    background->setScale(0.8 * Common::scaleSizeXY());
    background->setScaleY(1 * Common::scaleSizeY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_fight.png");
    header->setPosition(Common::scalePos(Vec2(150, 435)));
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);
    
    // Tạo các mục shop
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("A D to Move", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("Space to Jump", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel3 = MenuItemLabel::create(
        Label::createWithSystemFont("E to throw Stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel4 = MenuItemLabel::create(
        Label::createWithSystemFont("Q to throw Eagle", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel5 = MenuItemLabel::create(
        Label::createWithSystemFont("1 to use Mana Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel6 = MenuItemLabel::create(
        Label::createWithSystemFont("2 to use Health Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel7 = MenuItemLabel::create(
        Label::createWithSystemFont("Left mouse to Slash", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel8 = MenuItemLabel::create(
        Label::createWithSystemFont("I to open Charm", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    auto itemLabel9 = MenuItemLabel::create(
        Label::createWithSystemFont("J to show Attributes", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    
    // Vị trí các mục nằm trong khung
    int posY = 390;
    int space = 48;
    itemLabel1->setPosition(Vec2(150, posY) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel1->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel2->setPosition(Vec2(150, posY- space) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel2->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel3->setPosition(Vec2(150, posY- space*2) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel3->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel4->setPosition(Vec2(150, posY - space*3) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel4->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel5->setPosition(Vec2(150, posY - space*4) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel5->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel6->setPosition(Vec2(150, posY - space*5) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel6->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel7->setPosition(Vec2(150, posY - space*6) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel7->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel8->setPosition(Vec2(150, posY - space * 7) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel8->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel9->setPosition(Vec2(150, posY - space * 8) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel9->setColor(cocos2d::Color3B(0, 0, 0));

    auto menuItem = Menu::create(itemLabel1, itemLabel2, itemLabel3, itemLabel4, itemLabel5, itemLabel6, itemLabel7, itemLabel8, itemLabel9, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    layer->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Ok", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(GUILayer::menuOKCallback, this)
    );
    okItem->setPosition(Vec2(150, -30)* Common::scaleSizeXY());
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    auto menu = Menu::create(okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    layer->addChild(menu); // Thêm menu vào khung

    this->setName("popup");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void GUILayer::menuOKCallback(cocos2d::Ref* pSender) {
    this->removeFromParentAndCleanup(true);  
}
