/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "MenuScene.h"

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), "sound/bg1.mp3"));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    // Background
    auto background = Sprite::create("UI/backgroundwk.png");
    background->setPosition(Common::getCenter());
    Common::scaleAll(background, 1);
    addChild(background, 0);

    //Tao menu
    auto menuImages1 = MenuItemImage::create("UI/play.png", "UI/yellow/play.png",
        CC_CALLBACK_1(MenuScene::onClickMenuItem, this));
    menuImages1->setTag(1);

    auto menuImages2 = MenuItemImage::create("UI/guide.png", "UI/yellow/guide.png",
        CC_CALLBACK_1(MenuScene::guide, this));
    menuImages2->setTag(2);

    auto menuImages3 = MenuItemImage::create("UI/shop.png", "UI/yellow/shop.png",
        CC_CALLBACK_1(MenuScene::shop, this));
    menuImages3->setTag(3);

    auto menuImages4 = MenuItemImage::create("UI/setting.png", "UI/yellow/setting.png",
        CC_CALLBACK_1(MenuScene::setting, this));
    menuImages4->setTag(4);

    auto menuImages5 = MenuItemImage::create("UI/quit.png", "UI/yellow/quit.png",
        CC_CALLBACK_1(MenuScene::menuCloseCallback, this));
    menuImages5->setTag(5);

    auto mainMenu = Menu::create(menuImages1, menuImages2, menuImages3, menuImages4, menuImages5,nullptr);
    mainMenu->setScale(0.25 * Common::scaleSizeXY());
    mainMenu->setAnchorPoint(Vec2::ZERO);
    mainMenu->setPosition(Common::getCenter());
    mainMenu->alignItemsVerticallyWithPadding(70);
    
    addChild(mainMenu);
    zoomAction(mainMenu);

    //Label
    auto label = Label::createWithTTF("Dream World", "fonts/Marker Felt.ttf", 100);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setScale(Common::scaleSizeXY());
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - (label->getContentSize().height + 50) * Common::scaleSizeY()));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
    // Schedule the update method
    this->scheduleUpdate();
    return true;
}
void MenuScene::update(float dt) {
    settingInit->loadSettingData();
    AudioEngine::setVolume(settingInit->getBgMusicId() , settingInit->getVolume());
}
void MenuScene::onClickMenuItem(Ref* sender) {
    loadingbar();
}
void MenuScene::shop(Ref* sender) {
    Director::getInstance()->pushScene(ShopScene::createScene());
}
void MenuScene::setting(Ref* sender) {
    Director::getInstance()->pushScene(SettingScene::createScene());
}

void MenuScene::guide(Ref* sender) {
    Director::getInstance()->pushScene(GUIScene::createScene());
}

void MenuScene::zoomAction(Menu* item) {
    auto zoomIn = ScaleBy::create(1.0f, 1.1f);
    auto acSq = Sequence::create(zoomIn, zoomIn->reverse(), nullptr);
    auto actionRepeat = RepeatForever::create(acSq);
    item->runAction(actionRepeat);

}
void MenuScene::loadingbar() {
    // loading bar
    auto loadingBar = ui::LoadingBar::create("stick.png");
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    loadingBar->setPosition(Vec2(Common::getCenter().x, 100 * Common::scaleSizeY()));
    Common::scaleAll(loadingBar, 0.07);
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);// set huong chay
    loadingBar->setPercent(0);
    addChild(loadingBar);

    // chay loading bar theo thoi gian
    this->schedule([=](float delta) {
        float percent = loadingBar->getPercent();// 0 khi bat dau
        percent++;//tang dan 1 don vi
        loadingBar->setPercent(percent);// cap nhat lai vi tri
        if (percent >= 100.0f) {
            this->unschedule("updateLoadingBar");// ngung lai
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            // chuyen scene
            //auto villageScene = Map1Scene::createScene("map/bglv1.png", "sound/background2.mp3", "map1", true);
            //auto villageScene = Map2Scene::createScene("map/bglv1.png", "sound/bg2.mp3", "map2", true);
            //auto villageScene  = Boss1Scene::createScene("map/bglv1.png", "Enemy/Bossmap1/sound/bg.mp3", "boss1", false);
           // auto villageScene = Boss2Scene::createScene("map/bglv1.png", "Enemy/Bossmap2/sound/bg.mp3", "boss2", false);
           
           //auto villageScene = VillageScene::createScene("map/bglv1.png", "sound/bg1.mp3", "village", false);
            //auto villageScene =Boss3Scene::createScene("map/bg2.png", "Enemy/Bossmap3/sound/bg.mp3", "boss3", false);
            auto villageScene = Map3Scene::createScene("map/bg2.png", "sound/bg3.mp3", "map3", true);
            Director::getInstance()->replaceScene(villageScene);
        }
        }, 0.01f, "updateLoadingBar");// thoi gian chay 1% la 0,01s, ten cua schedule la "updateLoadingBar"
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}


