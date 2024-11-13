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

#include "ShopScene.h"
#include <string>
#include <thread>
#include "ui\CocosGUI.h"
#include "main/Common.h"
using namespace common;
using namespace std;
USING_NS_CC;

Scene* ShopScene::createScene()
{
    return ShopScene::create();
}


// on "init" you need to initialize your instance
bool ShopScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    // Background
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
    auto headerLabel = Label::createWithTTF("SHOP", "fonts/Marker Felt.ttf", 60 * Common::scaleSprite());
    headerLabel->setPosition(Vec2(Common::getCenter().x, visibleSize.height - headerLabel->getContentSize().height - 150 * Common::scaleYSprite()));
    this->addChild(headerLabel);

    int space = 80 * Common::scaleSprite();

    // Tạo các mục shop
    auto itemLabel0 = MenuItemLabel::create(
        Label::createWithSystemFont("The store is under maintenance!", "fonts/Marker Felt.ttf", 35 * Common::scaleSprite())
    );
    itemLabel0->setPosition(Vec2(Common::getCenter().x, headerLabel->getPositionY() - 100 * Common::scaleYSprite()));

    // Tạo các mục shop
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("Skin A - 3$", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    itemLabel1->setPosition(Vec2(itemLabel0->getPositionX(), itemLabel0->getPositionY() - space * 1));

    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("1000 Gem - 1$", "fonts/Marker Felt.ttf", 30 * Common::scaleSprite())
    );
    itemLabel2->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 2));

    // Ok
    auto menuImages1 = MenuItemImage::create("UI/scale/ok.png", "UI/scale/ok.png",
        CC_CALLBACK_1(ShopScene::goBack, this));
    menuImages1->setPosition(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 6);
    Common::scaleSprite(menuImages1, 0.05);
    // Cancel
    auto menuImages2 = MenuItemImage::create("UI/scale/cancel.png", "UI/scale/cancel.png",
        CC_CALLBACK_1(ShopScene::goBack, this));
    menuImages2->setPosition(Common::getCenter().x + 250 * Common::scaleSprite(), headerLabel->getPositionY());
    Common::scaleSprite(menuImages2,0.04);
    // Tạo menu
    auto menu = Menu::create(itemLabel0, itemLabel1, itemLabel2, menuImages1, menuImages2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    Common::zoomAction(menuImages2);
    Common::zoomAction(menuImages1);

    return true;
}


void ShopScene::goBack(Ref* sender) {
    // Quay lại scene trước đó
    Director::getInstance()->popScene();
}


