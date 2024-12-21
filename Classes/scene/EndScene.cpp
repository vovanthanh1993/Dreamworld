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

    //// Phát nhạc nền
    //settingInit->loadSettingData();
    //settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), bgMusic));

  
    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/end.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    // Schedule the update method
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








