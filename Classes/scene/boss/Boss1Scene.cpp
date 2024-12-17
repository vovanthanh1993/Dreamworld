#include "Boss1Scene.h"

using namespace std;
USING_NS_CC;

Scene* Boss1Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Boss1Scene* ret = new Boss1Scene();
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
bool Boss1Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }

    contactListener->bossmap1 = bossmap1;
    Common::addMapName(this, "Lonely Rock");
    return true;
}

// update
void Boss1Scene::update(float dt) {
    if (!player->isAlive) return;
    BaseScene::update(dt);

    //-------------------CAP NHAT LAI SPRITE--------------------------
    // Boss map 1
    if (bossmap1->isAlive) {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume() * 0.2);
    }
    if (contactListener->isNext && !bossmap1->isAlive) {
        player->savePlayerDataInit();
        auto newScene = Map2Scene::createScene("map/bglv1.png", "sound/bg2.mp3", "map2", true);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;
    settingInit->loadSettingData();
    
}

void Boss1Scene::spawnObject() {
    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();

    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                player->init(false);
                break;
            }
        }
    }
    // spawn boss
    auto bossLayer = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = bossLayer->getTileAt(Vec2(x, y));
            if (tile) {
                bossmap1 = new BossMap1(world, this, bodyToSpriteMap);
                bossmap1->player = player;
                bossmap1->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                break;
            }
        }
    }

    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





