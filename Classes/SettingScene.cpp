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

#include "VillageScene.h"
#include "SettingScene.h"
#include <string>
#include <thread>
#include "ui\CocosGUI.h"
#include "MenuScene.h"
#include "gui/Setting.h"

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
    setting->loadSettingData();

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Background
    auto background = Sprite::create("UI/backgroundwk.png");
    background->setPosition(Common::getCenter());
    Common::scaleSprite(background, 1);
    addChild(background, 0);

    // Background
    auto backgroundSetting = Sprite::create("UI/scale/bg2.png");
    backgroundSetting->setPosition(Common::getCenter());
    Common::scaleSprite(backgroundSetting, 0.4);
    addChild(backgroundSetting, 0);

    // Tạo tiêu đề
    auto headerLabel = Label::createWithTTF("SETTING", "fonts/Marker Felt.ttf", 60 * Common::scaleSprite());
    headerLabel->setPosition(Vec2(Common::getCenter().x, visibleSize.height - headerLabel->getContentSize().height - 150* Common::scaleSprite()));
    this->addChild(headerLabel);
    
    int space = 80 * Common::scaleXSprite();

    // Tạo tiêu đề
    auto musicLabel = Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 35 * Common::scaleSprite());
    musicLabel->setPosition(Vec2(Common::getCenter().x- 200 * Common::scaleXSprite(), headerLabel->getPositionY() - 200 * Common::scaleYSprite()));
    musicLabel->setAnchorPoint(Vec2(0, 0));

    this->addChild(musicLabel);
    // Tạo slider cho âm lượng
    auto volumeSlider = ui::Slider::create();
    volumeSlider->setPercent(setting->getVolume()*100); // Khởi tạo
    volumeSlider->loadBarTexture("UI/scale/bar_bg.png");
    volumeSlider->loadSlidBallTextures("UI/scale/button.png", "UI/scale/button.png", "");
    volumeSlider->loadProgressBarTexture("UI/scale/bar.png");
    volumeSlider->addEventListener(CC_CALLBACK_2(SettingScene::sliderEvent, this));
    volumeSlider->setPosition(Vec2(Common::getCenter().x-20 * Common::scaleSprite(), musicLabel->getPositionY()));
    volumeSlider->getSlidBallRenderer()->setScaleX(0.35 * Common::scaleXSprite());
    Common::scaleSprite(volumeSlider, 0.03);
    volumeSlider->setAnchorPoint(Vec2(0, 0));
    this->addChild(volumeSlider);


    // Tạo tiêu đề
    auto gameplayVolume = Label::createWithTTF("Effect", "fonts/Marker Felt.ttf", 35);
    gameplayVolume->setPosition(Vec2(musicLabel->getPosition().x, musicLabel->getPositionY() - space));
    gameplayVolume->setAnchorPoint(Vec2(0, 0));
    gameplayVolume->setScale(Common::scaleSprite());
    this->addChild(gameplayVolume);

    // Tạo slider cho âm lượng
    auto volumeSliderGameplay = ui::Slider::create();
    volumeSliderGameplay->setPercent(setting->getGameplayVol() * 100); // Khởi tạo
    volumeSliderGameplay->loadBarTexture("UI/scale/bar_bg.png");
    volumeSliderGameplay->loadSlidBallTextures("UI/scale/button.png", "UI/scale/button.png", "UI/scale/button.png");
    volumeSliderGameplay->loadProgressBarTexture("UI/scale/bar.png");
    volumeSliderGameplay->addEventListener(CC_CALLBACK_2(SettingScene::sliderEventGamePlay, this));
    volumeSliderGameplay->setPosition(Vec2(volumeSlider->getPositionX(), gameplayVolume->getPositionY()));
    volumeSliderGameplay->getSlidBallRenderer()->setScaleX(0.35* Common::scaleXSprite());
    Common::scaleSprite(volumeSliderGameplay, 0.03);
    //volumeSliderGameplay->setScale(Common::scaleSprite());
    
    volumeSliderGameplay->setAnchorPoint(Vec2(0, 0));
    this->addChild(volumeSliderGameplay);

    // Volume
    // Tạo tiêu đề
    auto volumeLabel = Label::createWithTTF("Sub", "fonts/Marker Felt.ttf", 35 * Common::scaleSprite());
    volumeLabel->setAnchorPoint(Vec2(0, 0));
    volumeLabel->setPosition(Vec2(musicLabel->getPositionX(), musicLabel->getPositionY() - space*2));
    this->addChild(volumeLabel);
    // Tạo hình ảnh cho các trạng thái của nút toggle
    auto volumeOn = Sprite::create("UI/scale/button_on.png");
    auto volumeOff = Sprite::create("UI/scale/button_off.png");

    // Tạo MenuItemToggle
    auto toggleItem = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingScene::toggleSubCallback, this),
        MenuItemSprite::create(volumeOff, volumeOff),
        MenuItemSprite::create(volumeOn, volumeOn),
        nullptr
    );
    toggleItem->setSelectedIndex(setting->getSub());
    toggleItem->setAnchorPoint(Vec2(0, 0));
    toggleItem->setPosition(Vec2(volumeSlider->getPositionX(), volumeLabel->getPositionY()));
    //toggleItem->setScale(Common::scaleSprite());
    Common::scaleSprite(toggleItem, 0.04);

    // Sound
    // Tạo tiêu đề
    auto soundLabel = Label::createWithTTF("V Sync", "fonts/Marker Felt.ttf", 35 * Common::scaleSprite());
    soundLabel->setAnchorPoint(Vec2(0, 0));
    soundLabel->setPosition(Vec2(musicLabel->getPositionX(), musicLabel->getPositionY() - space*3));
    this->addChild(soundLabel);
    // Tạo hình ảnh cho các trạng thái của nút toggle
    auto soundOn = Sprite::create("UI/scale/button_on.png");
    auto soundOff = Sprite::create("UI/scale/button_off.png");
    // Tạo MenuItemToggle
    auto toggleItemSound = MenuItemToggle::createWithCallback(
        CC_CALLBACK_1(SettingScene::toggleVsynCallback, this),
        MenuItemSprite::create(soundOff, soundOff),
        MenuItemSprite::create(soundOn, soundOn),
        nullptr
    );
    toggleItemSound->setSelectedIndex(setting->getVsyn());
    toggleItemSound->setAnchorPoint(Vec2(0, 0));
    toggleItemSound->setPosition(Vec2(volumeSlider->getPositionX(), soundLabel->getPositionY()));
    //toggleItemSound->setScale(Common::scaleSprite());
    Common::scaleSprite(toggleItemSound, 0.04);

    // Ok
    auto menuImages1 = MenuItemImage::create("UI/scale/ok.png", "UI/scale/ok.png",
        CC_CALLBACK_1(SettingScene::save, this));
    menuImages1->setPosition(headerLabel->getPositionX(), musicLabel->getPositionY() - space * 6);
    Common::scaleSprite(menuImages1, 0.05);
    // Cancel
    auto menuImages2 = MenuItemImage::create("UI/scale/cancel.png", "UI/scale/cancel.png",
        CC_CALLBACK_1(SettingScene::goBack, this));
    menuImages2->setPosition(Common::getCenter().x + 250 * Common::scaleSprite(), headerLabel->getPositionY());
    Common::scaleSprite(menuImages2, 0.04);

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
    setting->setSub(selectedIndex);
    if (selectedIndex == 0) {
        
    }
    else {
        CCLOG("Sound ON");
        // Bật âm thanh
    }
}

void SettingScene::toggleVsynCallback(Ref* sender) {
    auto toggleItem = dynamic_cast<MenuItemToggle*>(sender);
    int selectedIndex = toggleItem->getSelectedIndex();
    setting->setVsyn(selectedIndex);
    if (selectedIndex == 0) {
        
    }
    else {
        CCLOG("Sound ON");
        // Bật âm thanh
    }
}


void SettingScene::sliderEvent(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        auto slider = dynamic_cast<ui::Slider*>(sender);
        setting->setVolume(slider->getPercent() / 100.0f); // Chuyển đổi sang khoảng 0-1
        setVolume(); // Gọi hàm để thiết lập âm lượng
    }
}

void SettingScene::sliderEventGamePlay(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        auto slider = dynamic_cast<ui::Slider*>(sender);
        setting->setGameplayVol(slider->getPercent() / 100.0f); // Chuyển đổi sang khoảng 0-1
    }
}

void SettingScene::setVolume() {
    //setting->saveSettingData();
}

void SettingScene::goBack(Ref* sender) {
    // Quay lại scene trước đó
    Director::getInstance()->popScene();
}

void SettingScene::save(Ref* sender) {
    setting->saveSettingData();
    Director::getInstance()->popScene();
}
