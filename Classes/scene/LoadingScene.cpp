#include "LoadingScene.h"
#include "VillageScene.h"

LoadingScene* LoadingScene::create()
{
    LoadingScene* ret = new LoadingScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool LoadingScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    AudioEngine::stopAll();
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    auto centerPosition = cocos2d::Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2);

    // Background
    auto backgroundAll = Sprite::create("UI/backgroundwk.png");
    backgroundAll->setPosition(Common::getCenter());
    backgroundAll->setOpacity(150);
    Common::scaleAll(backgroundAll, 1);
    addChild(backgroundAll, 0);

    // Thêm background
    auto background = cocos2d::Sprite::create("ui/loadingbackdround1.png");
    background->setScale(0.2* Common::scaleSizeXY());
    background->setScaleX(0.3 * Common::scaleSizeXY());
    //background->setAnchorPoint(Vec2(1,0));
    background->setPosition(origin.x + visibleSize.width/2, 20 * Common::scaleSizeY());
    this->addChild(background);
    background->setOpacity(170);

    Common::showTextRandom(this, loadingtext, 5);

    // Tạo thanh tiến trình
    auto progressBar = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("ui/loadingprocess1.png"));
    progressBar->setOpacity(170);
    //progressBar->setAnchorPoint(Vec2(1, 0));
    progressBar->setScale(0.2 * Common::scaleSizeXY());
    progressBar->setScaleX(0.3 * Common::scaleSizeXY());
    progressBar->setType(cocos2d::ProgressTimer::Type::BAR);
    progressBar->setMidpoint(cocos2d::Vec2(0, 0));
    progressBar->setBarChangeRate(cocos2d::Vec2(1, 0));
    progressBar->setPercentage(0);
    progressBar->setPosition(origin.x + visibleSize.width/2, 20 * Common::scaleSizeY());
    this->addChild(progressBar);

    // Tiến trình tải và chuyển cảnh
    auto delay = cocos2d::DelayTime::create(2.0f); // Thời gian giả lập
    auto progressAction = cocos2d::ProgressTo::create(2.0f, 100);

    progressBar->runAction(progressAction);

    this->runAction(cocos2d::Sequence::create(
        delay,
        cocos2d::CallFunc::create([=]() { auto villageScene = VillageScene::createScene("map/bglv1.png", "sound/bg1.mp3", "village", false);
        cocos2d::Director::getInstance()->replaceScene(villageScene); }),
        nullptr
    ));

    return true;
}

void LoadingScene::loadResourcesAndSwitchScene()
{
    auto villageScene = VillageScene::createScene("map/bglv1.png", "sound/bg1.mp3", "village", false);
    cocos2d::Director::getInstance()->replaceScene(villageScene);
}
