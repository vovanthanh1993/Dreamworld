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
#include "SettingScene.h"

using namespace std;
using namespace common;
USING_NS_CC;

Scene* SettingScene::createScene()
{
    return SettingScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    SettingManager::getInstance()->loadSettingData();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Background
    auto background = Sprite::create("UI/backgroundwk.png");
    background->setPosition(Common::getCenter());
    Common::scaleAll(background, 1);
    addChild(background, 0);

    // Background
    auto backgroundSetting = Sprite::create("UI/scale/bg2.png");
    backgroundSetting->setPosition(Common::getCenter());
    Common::scaleAll(backgroundSetting, 0.4);
    addChild(backgroundSetting, 0);

    // Tạo tiêu đề
    auto headerLabel = Label::createWithTTF("SETTING", "fonts/Marker Felt.ttf", 60 * Common::scaleSizeXY());
    headerLabel->setPosition(Vec2(Common::getCenter().x, visibleSize.height - headerLabel->getContentSize().height - 150* Common::scaleSizeXY()));
    this->addChild(headerLabel);
    
    int space = 80 * Common::scaleSizeX();

    // Tạo tiêu đề
    auto musicLabel = Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 35 * Common::scaleSizeXY());
    musicLabel->setPosition(Vec2(Common::getCenter().x- 200 * Common::scaleSizeX(), headerLabel->getPositionY() - 200 * Common::scaleSizeY()));
    musicLabel->setAnchorPoint(Vec2(0, 0));

    this->addChild(musicLabel);
    // Tạo slider cho âm lượng
    auto volumeSliderMusic = ui::Slider::create();
    volumeSliderMusic->setPercent(SettingManager::getInstance()->getVolume()*100); // Khởi tạo
    volumeSliderMusic->loadBarTexture("UI/scale/bar_bg.png");
    volumeSliderMusic->loadSlidBallTextures("UI/scale/button.png", "UI/scale/button.png", "");
    volumeSliderMusic->loadProgressBarTexture("UI/scale/bar.png");
    volumeSliderMusic->addEventListener(CC_CALLBACK_2(SettingScene::changeMucsicVolume, this));
    volumeSliderMusic->setPosition(Vec2(Common::getCenter().x-20 * Common::scaleSizeXY(), musicLabel->getPositionY()));
    volumeSliderMusic->getSlidBallRenderer()->setScaleX(0.35 * Common::scaleSizeX());
    Common::scaleAll(volumeSliderMusic, 0.03);
    volumeSliderMusic->setAnchorPoint(Vec2(0, 0));
    this->addChild(volumeSliderMusic);

    // Tạo tiêu đề
    auto gameplayVolume = Label::createWithTTF("Effect", "fonts/Marker Felt.ttf", 35);
    gameplayVolume->setPosition(Vec2(musicLabel->getPosition().x, musicLabel->getPositionY() - space));
    gameplayVolume->setAnchorPoint(Vec2(0, 0));
    gameplayVolume->setScale(Common::scaleSizeXY());
    this->addChild(gameplayVolume);

    // Tạo slider cho âm lượng
    auto volumeSliderEffect = ui::Slider::create();
    volumeSliderEffect->setPercent(SettingManager::getInstance()->getGameplayVol() * 100); // Khởi tạo
    volumeSliderEffect->loadBarTexture("UI/scale/bar_bg.png");
    volumeSliderEffect->loadSlidBallTextures("UI/scale/button.png", "UI/scale/button.png", "UI/scale/button.png");
    volumeSliderEffect->loadProgressBarTexture("UI/scale/bar.png");
    volumeSliderEffect->addEventListener(CC_CALLBACK_2(SettingScene::changeEffectVolume, this));
    volumeSliderEffect->setPosition(Vec2(volumeSliderMusic->getPositionX(), gameplayVolume->getPositionY()));
    volumeSliderEffect->getSlidBallRenderer()->setScaleX(0.35* Common::scaleSizeX());
    Common::scaleAll(volumeSliderEffect, 0.03);
    
    volumeSliderEffect->setAnchorPoint(Vec2(0, 0));
    this->addChild(volumeSliderEffect);

    // Sub
    auto subLabel = Label::createWithTTF("Sub", "fonts/Marker Felt.ttf", 35 * Common::scaleSizeXY());
    subLabel->setAnchorPoint(Vec2(0, 0));
    subLabel->setPosition(Vec2(musicLabel->getPositionX(), musicLabel->getPositionY() - space*2));
    this->addChild(subLabel);
    // Tạo hình ảnh cho các trạng thái của nút toggle
    auto subOn = Sprite::create("UI/scale/button_on.png");
    auto subOff = Sprite::create("UI/scale/button_off.png");

    // Tạo MenuItemToggle
    auto toggleItem = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingScene::toggleSubCallback, this),
        MenuItemSprite::create(subOff, subOff),
        MenuItemSprite::create(subOn, subOn),
        nullptr
    );
    toggleItem->setSelectedIndex(SettingManager::getInstance()->getSub());
    toggleItem->setAnchorPoint(Vec2(0, 0));
    toggleItem->setPosition(Vec2(volumeSliderMusic->getPositionX(), subLabel->getPositionY()));
    Common::scaleAll(toggleItem, 0.04);

    // V Sync
    auto vsyncLabel = Label::createWithTTF("V Sync", "fonts/Marker Felt.ttf", 35 * Common::scaleSizeXY());
    vsyncLabel->setAnchorPoint(Vec2(0, 0));
    vsyncLabel->setPosition(Vec2(musicLabel->getPositionX(), musicLabel->getPositionY() - space*3));
    this->addChild(vsyncLabel);
    // Tạo hình ảnh cho các trạng thái của nút toggle
    auto vsyncOn = Sprite::create("UI/scale/button_on.png");
    auto vsyncOff = Sprite::create("UI/scale/button_off.png");
    // Tạo MenuItemToggle
    auto toggleItemSound = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingScene::toggleVsynCallback, this),
        MenuItemSprite::create(vsyncOff, vsyncOff),
        MenuItemSprite::create(vsyncOn, vsyncOn),
        nullptr
    );
    toggleItemSound->setSelectedIndex(SettingManager::getInstance()->getVsyn());
    toggleItemSound->setAnchorPoint(Vec2(0, 0));
    toggleItemSound->setPosition(Vec2(volumeSliderMusic->getPositionX(), vsyncLabel->getPositionY()));
    Common::scaleAll(toggleItemSound, 0.04);

    // Ok
    auto menuImages1 = MenuItemImage::create("UI/scale/ok.png", "UI/scale/ok.png",
        CC_CALLBACK_1(SettingScene::save, this));
    menuImages1->setPosition(headerLabel->getPositionX(), musicLabel->getPositionY() - space * 6);
    Common::scaleAll(menuImages1, 0.05);
    // Cancel
    auto menuImages2 = MenuItemImage::create("UI/scale/cancel.png", "UI/scale/cancel.png",
        CC_CALLBACK_1(SettingScene::close, this));
    menuImages2->setPosition(Common::getCenter().x + 250 * Common::scaleSizeXY(), headerLabel->getPositionY());
    Common::scaleAll(menuImages2, 0.04);

    // Tạo menu
    auto menu = Menu::create(toggleItem, toggleItemSound, menuImages1, menuImages2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    Common::zoomAction(menuImages2);
    Common::zoomAction(menuImages1);
    return true;
}
void SettingScene::toggleSubCallback(Ref* sender) {
    auto toggleItem = dynamic_cast<MenuItemToggle*>(sender);
    int selectedIndex = toggleItem->getSelectedIndex();
    SettingManager::getInstance()->setSub(selectedIndex);
}

void SettingScene::toggleVsynCallback(Ref* sender) {
    auto toggleItem = dynamic_cast<MenuItemToggle*>(sender);
    int selectedIndex = toggleItem->getSelectedIndex();
    SettingManager::getInstance()->setVsyn(selectedIndex);
}


void SettingScene::changeMucsicVolume(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        auto slider = dynamic_cast<ui::Slider*>(sender);
        SettingManager::getInstance()->setVolume(slider->getPercent() / 100.0f); // To save to file
        MusicManager::getInstance()->setVolume(slider->getPercent() / 100.0f);
    }
}

void SettingScene::changeEffectVolume(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        auto slider = dynamic_cast<ui::Slider*>(sender);
        SettingManager::getInstance()->setGameplayVol(slider->getPercent() / 100.0f); // // To save to file
    }
}

void SettingScene::close(Ref* sender) {
    Director::getInstance()->popScene();
}

void SettingScene::save(Ref* sender) {
    SettingManager::getInstance()->saveSettingData();
    Director::getInstance()->popScene();
}
