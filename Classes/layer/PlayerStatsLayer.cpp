#include "PlayerStatsLayer.h"

USING_NS_CC;
PlayerStatsLayer* PlayerStatsLayer::createLayer(Player* player, Scene* scene)
{
    PlayerStatsLayer* ret = new PlayerStatsLayer();
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
bool PlayerStatsLayer::init(Player* player, Scene* scene) {

    this->player = player;
    player->isEnable = false;
    player->getBody()->SetLinearVelocity(b2Vec2_zero);
    player->idle();

    Size screenSize = Director::getInstance()->getVisibleSize();
    // Tạo khung nhỏ cho shop
    auto layer = LayerColor::create(Color4B(0, 0, 0, 0), 300* Common::scaleSizeX(), 400 * Common::scaleSizeY());
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

     //Thêm hình nền cho khung
    auto background = Sprite::create("ui/shopbg.png"); // Đường dẫn đến hình nền
    background->setPosition(Common::scalePos(Vec2(150, 200))); // Giữa khung
    background->setScale(0.8 * Common::scaleSizeXY());
    layer->addChild(background, -1); // Thêm hình nền dưới các mục khác

    auto header = Sprite::create("ui/header_fight.png");
    //auto header = Sprite::create("ui/head2.png"); 
    header->setPosition(Vec2(150 * Common::scaleSizeX(), 370 * Common::scaleSizeY()));
    header->setScale(Common::scaleSizeXY());
    layer->addChild(header);
    
    // Tạo các mục shop
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("Health: " + to_string((int)player->getHealth()), "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel1->setAnchorPoint(Vec2(0, 0.5));
    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("Mana: " + to_string((int)player->getMana()), "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel2->setAnchorPoint(Vec2(0, 0.5));
    auto itemLabel3 = MenuItemLabel::create(
        Label::createWithSystemFont("Slash damage: " + to_string((int)player->slashDamage), "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel3->setAnchorPoint(Vec2(0, 0.5));
    auto itemLabel4 = MenuItemLabel::create(
        Label::createWithSystemFont("Stick damage: " + to_string((int)player->stickDamage), "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel4->setAnchorPoint(Vec2(0, 0.5));
    auto itemLabel5 = MenuItemLabel::create(
        Label::createWithSystemFont("Eagle damage: " + to_string((int)player->eagleDamage), "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel5->setAnchorPoint(Vec2(0, 0.5));
    
    // Vị trí các mục nằm trong khung
    int posX = 10;
    int posY = 330;
    int spaceY = 50;
    itemLabel1->setPosition(Vec2(posX, posY) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel1->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel2->setPosition(Vec2(posX, posY - spaceY) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel2->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel3->setPosition(Vec2(posX, posY - spaceY*2) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel3->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel4->setPosition(Vec2(posX, posY - spaceY*3) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel4->setColor(cocos2d::Color3B(0, 0, 0));
    itemLabel5->setPosition(Vec2(posX, posY - spaceY*4) * Common::scaleSizeXY()); // Vị trí trong khung
    itemLabel5->setColor(cocos2d::Color3B(0, 0, 0));

    auto menuItem = Menu::create(itemLabel1, itemLabel2, itemLabel3, itemLabel4, itemLabel5, nullptr);
    menuItem->setPosition(Vec2::ZERO);
    layer->addChild(menuItem); // Thêm menu vào layer

    // Thêm nút trở về
    auto okItem = MenuItemLabel::create(
        Label::createWithSystemFont("Ok", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY()),
        CC_CALLBACK_1(PlayerStatsLayer::menuOKCallback, this)
    );
    okItem->setPosition(Vec2(150* Common::scaleSizeX(), 35* Common::scaleSizeY()));
    okItem->setColor(cocos2d::Color3B(180, 57, 67));
    auto menu = Menu::create(okItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    layer->addChild(menu); // Thêm menu vào khung

    this->setName("popup");
    this->addChild(layer);
    scene->addChild(this, 100);
    return true;
}

void PlayerStatsLayer::menuOKCallback(cocos2d::Ref* pSender) {
    this->removeFromParentAndCleanup(true);  
}
