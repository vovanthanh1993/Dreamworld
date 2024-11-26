#include "PauseMenu.h"


USING_NS_CC;

PauseMenu::PauseMenu(Vec2 position) {
    // Create menu
    auto menuImages1 = MenuItemImage::create("UI/continue.png", "UI/yellow/continue.png",
        CC_CALLBACK_1(PauseMenu::resumeGame, this));
    menuImages1->setTag(1);

    auto menuImages2 = MenuItemImage::create("UI/setting.png", "UI/yellow/setting.png",
        CC_CALLBACK_1(PauseMenu::setting, this));
    menuImages2->setTag(1);

    auto menuImages3 = MenuItemImage::create("UI/menu.png", "UI/yellow/menu.png",
        CC_CALLBACK_1(PauseMenu::returnMenu, this));
    menuImages3->setTag(2);

    auto menuImages4 = MenuItemImage::create("UI/quit.png", "UI/yellow/quit.png",
        CC_CALLBACK_1(PauseMenu::quitGame, this));
    menuImages4->setTag(3);


    // Tạo menu và thêm nút vào menu
    menu = Menu::create(menuImages1, menuImages2,menuImages3, menuImages4, nullptr);
    menu->setScale(0.25* Common::scaleSizeXY());
    menu->setAnchorPoint(Vec2::ZERO);
    menu->alignItemsVerticallyWithPadding(70);
    menu->setPosition(position); // Đặt menu ở giữa màn hình
    menu->setName("Menu");
}

void PauseMenu::returnMenu(Ref* sender) {
    Director::getInstance()->resume();
    auto newScene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
}

void PauseMenu::resumeGame(Ref* sender) {
    Director::getInstance()->resume();
    menu->removeFromParentAndCleanup(true);
}

void PauseMenu::setting(Ref* sender) {
    Director::getInstance()->pushScene(SettingScene::createScene());
}

void PauseMenu::quitGame(Ref* sender) {
    Director::getInstance()->end();
}
