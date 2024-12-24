#include "EndScene.h"

using namespace std;
USING_NS_CC;

Scene* EndScene::createScene()
{
    EndScene* ret = new EndScene();
    if (ret && ret->init()) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

//on "init" you need to initialize your instance
bool EndScene::init()
{

    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // Phát nhạc nền
    SettingManager::getInstance()->loadSettingData();
    MusicManager::getInstance()->playBackgroundMusic("sound/endbg.mp3", true);
    MusicManager::getInstance()->setVolume(SettingManager::getInstance()->getVolume());

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/endbg.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    auto origin = Director::getInstance()->getVisibleOrigin();
    Size screenSize = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithTTF("Press ESC to return menu", "fonts/Marker Felt.ttf", 20 * Common::scaleSizeXY());
    label->setAnchorPoint(Vec2(1, 0));
    label->setPosition(origin.x + screenSize.width, origin.y);
    this->addChild(label, 90);
    
    auto zoomIn = ScaleBy::create(1.0f, 1.1f);
    auto acSq = Sequence::create(zoomIn, zoomIn->reverse(), nullptr);
    auto actionRepeat = RepeatForever::create(acSq);
    label->runAction(actionRepeat);

    // Press
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(EndScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    this->scheduleUpdate();
    return true;
}


// update
void EndScene::update(float dt) {

    //BaseScene::update(dt);

    /*if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss4Scene::createScene("map/bg2.png", "sound/bg4.mp3", "boss4", false);
        if (player->isComplete()) {
            newScene  = VillageScene::createScene("map/bglv1.png", "sound/bg1.mp3", "village", false);;
        }
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
    }*/
}

void EndScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == (EventKeyboard::KeyCode::KEY_ESCAPE)) {
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}








