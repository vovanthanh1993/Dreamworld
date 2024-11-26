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
#include "GameOver.h"

using namespace std;
USING_NS_CC;

Scene* GameOver::createScene()
{
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    cocos2d::AudioEngine::stopAll();
    if (!Scene::init()) {
        return false;
    }
    
    // GameOver

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Background
    auto background = Sprite::create("UI/GameOver/bg.png");
    background->setPosition(Common::getCenter());
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    createGameOver(Vec2(Common::getCenter().x, visibleSize.height / 2 + 290 * Common::scaleSizeXY()));
    

    // Create menu
    auto menuImages1 = MenuItemImage::create("UI/continue.png", "UI/yellow/continue.png",
        CC_CALLBACK_1(GameOver::restartCallback, this));
    menuImages1->setTag(1);

    auto menuImages2 = MenuItemImage::create("UI/menu.png", "UI/yellow/menu.png",
        CC_CALLBACK_1(GameOver::returnMenu, this));
    menuImages2->setTag(2);
   
    auto menuImages3 = MenuItemImage::create("UI/quit.png", "UI/yellow/quit.png",
        CC_CALLBACK_1(GameOver::quitGame, this));
    menuImages3->setTag(3);


    // Tạo menu và thêm nút vào menu
    auto menu = Menu::create(menuImages1, menuImages2 , menuImages3, nullptr);
    menu->setScale(0.25 * Common::scaleSizeXY());
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Common::getCenter());
    menu->alignItemsVerticallyWithPadding(70);
    addChild(menu);

    return true;
}

void GameOver::restartCallback(Ref* sender) {

    auto scene = VillageScene::createScene("map/bglv1.png", "sound/background2.mp3", "village", false);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameOver::returnMenu(Ref * sender) {
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameOver::createGameOver(Vec2 pos) {
    auto spriteNode = SpriteBatchNode::create("UI/GameOver/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/GameOver/sprites.plist");
    auto sprite = Sprite::createWithSpriteFrameName("YouLose_0.png");
    sprite->setScale(1 * Common::scaleSizeXY());
    sprite->setPosition(pos);
    sprite->setTag(Constants::TAG_PLAYER);
    auto animate = Animate::create(Common::createAnimation("YouLose_", 29, 0.04));
    animate->retain();
    sprite->runAction(RepeatForever::create(animate));
    spriteNode->addChild(sprite);
    addChild(spriteNode);
}

void GameOver::quitGame(Ref* sender) {
    Director::getInstance()->end(); // Hoặc chuyển đến menu chính
}

