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

#include "GUIScene.h"

using namespace common;
using namespace std;
USING_NS_CC;

Scene* GUIScene::createScene()
{
    return GUIScene::create();
}


// on "init" you need to initialize your instance
bool GUIScene::init()
{
    if (!Scene::init()) {
        return false;
    }

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
    auto headerLabel = Label::createWithTTF("GUIDE", "fonts/Marker Felt.ttf", 60 * Common::scaleSizeXY());
    headerLabel->setPosition(Vec2(Common::getCenter().x, visibleSize.height - (headerLabel->getContentSize().height+150) * Common::scaleSizeY()));
    this->addChild(headerLabel);

    int space = 60 * Common::scaleSizeY();

    // Tạo các mục shop
    auto itemLabel0 = MenuItemLabel::create(
        Label::createWithSystemFont("The journey ahead is fraught with challenges!", "fonts/Marker Felt.ttf", 28 * Common::scaleSizeXY())
    );
    itemLabel0->setPosition(Vec2(Common::getCenter().x, headerLabel->getPositionY()- 100 * Common::scaleSizeY()));

    // Tạo các mục shop
    auto itemLabel1 = MenuItemLabel::create(
        Label::createWithSystemFont("A D to Move", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel1->setPosition(Vec2(itemLabel0->getPositionX(), itemLabel0->getPositionY() - space * 1));

    auto itemLabel2 = MenuItemLabel::create(
        Label::createWithSystemFont("Space to Jump", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel2->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 2));

    auto itemLabel3 = MenuItemLabel::create(
        Label::createWithSystemFont("I to Slash", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel3->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 3));

    auto itemLabel4 = MenuItemLabel::create(
        Label::createWithSystemFont("O to throw Stick", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    
    itemLabel4->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 4));

    auto itemLabel5 = MenuItemLabel::create(
        Label::createWithSystemFont("P to throw Eagle", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel5->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 5));
    auto itemLabel6 = MenuItemLabel::create(
        Label::createWithSystemFont("1 to use Health Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel6->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 6));

    auto itemLabel7 = MenuItemLabel::create(
        Label::createWithSystemFont("2 to use Mana Potion", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel7->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 7));

    auto itemLabel8 = MenuItemLabel::create(
        Label::createWithSystemFont("C to open Charm", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel8->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 8));

    auto itemLabel9 = MenuItemLabel::create(
        Label::createWithSystemFont("V to show Attributes", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel9->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 9));

    auto itemLabel10 = MenuItemLabel::create(
        Label::createWithSystemFont("E to speak with NPC", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY())
    );
    itemLabel10->setPosition(Vec2(itemLabel1->getPositionX(), itemLabel0->getPositionY() - space * 10));

    // Cancel
    auto menuImages2 = MenuItemImage::create("UI/scale/cancel.png", "UI/scale/cancel.png",
        CC_CALLBACK_1(GUIScene::goBack, this));
    menuImages2->setPosition(Common::getCenter().x + 250 * Common::scaleSizeXY(), headerLabel->getPositionY());
    Common::scaleAll(menuImages2, 0.04);

    // Tạo menu
    auto menu = Menu::create(itemLabel0, itemLabel1, itemLabel2, itemLabel3, itemLabel4, itemLabel5, itemLabel6, itemLabel7, itemLabel8, itemLabel9, itemLabel10, menuImages2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    Common::zoomAction(menuImages2);
    return true;
}


void GUIScene::goBack(Ref* sender) {
    // Quay lại scene trước đó
    Director::getInstance()->popScene();
}


