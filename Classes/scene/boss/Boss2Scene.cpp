#include "Boss2Scene.h"

using namespace std;
USING_NS_CC;

Scene* Boss2Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Boss2Scene* ret = new Boss2Scene();
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
bool Boss2Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }
    contactListener->bossmap2 = bossmap2;
    return true;
}

// update
void Boss2Scene::update(float dt) {
    if (!player->isAlive) return;
    BaseScene::update(dt);

    if (bossmap2->isAlive) {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume()*0.2);
    }

    //-------------------CAP NHAT LAI SPRITE--------------------------
    if (contactListener->isNext && !bossmap2->isAlive) {
        player->savePlayerDataInit();
        auto newScene = Map3Scene::createScene("map/bg2.png", "sound/bg3.mp3", "map3", true);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        
    }
    contactListener->isNext = false;
    settingInit->loadSettingData();
}

void Boss2Scene::spawnObject() {
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
                bossmap2 = new BossMap2(world, this, bodyToSpriteMap);
                bossmap2->player = player;
                bossmap2->map = map;
                bossmap2->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }

    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





