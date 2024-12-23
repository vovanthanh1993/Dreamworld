#include "Boss3Scene.h"

using namespace std;
USING_NS_CC;

Scene* Boss3Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Boss3Scene* ret = new Boss3Scene();
    if (ret && ret->init(bg, bgMusic, mapName, isMoveCamera)) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

//on "init" you need to initialize your instance
bool Boss3Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }
    contactListener->bossmap3 = bossmap3;
    return true;
}

// update
void Boss3Scene::update(float dt) {
    BaseScene::update(dt);

    if (bossmap3->isAlive) {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume()*0.2);
    }

    if (contactListener->isNext && !bossmap3->isAlive) {
        player->savePlayerDataInit();
        auto newScene = MikoScene::createScene("map/bg2.png", "sound/bg3.mp3", "mikomap", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        isEndMap = true;
    }
    contactListener->isNext = false;
    settingInit->loadSettingData();
}

void Boss3Scene::spawnObject() {
    Common::addMapName(this, "Darkness Bat");

    MapItem* item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();

    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x / Common::scaleSizeXY()+x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE)* Common::scaleSizeXY(), bodyToSpriteMap);
                player->init(false);
            }
        }
    }

    // spawn boss
    auto bossLayer = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = bossLayer->getTileAt(Vec2(x, y));
            if (tile) {
                bossmap3 = new BossMap3(world, this, bodyToSpriteMap);
                bossmap3->map = map;
                bossmap3->player = player;
                bossmap3->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }

    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





