#include "Boss4Scene.h"

using namespace std;
USING_NS_CC;

Scene* Boss4Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Boss4Scene* ret = new Boss4Scene();
    if (ret && ret->init(bg, bgMusic, mapName, isMoveCamera)) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

// update
void Boss4Scene::update(float dt) {
    BaseScene::update(dt);

    if (bossmap4->isAlive) {
        MusicManager::getInstance()->setVolume(SettingManager::getInstance()->getVolume());
    }
    else {
        MusicManager::getInstance()->setVolume(SettingManager::getInstance()->getVolume() * 0.2);
    }

    if (contactListener->isNext && !bossmap4->isAlive) {
        player->savePlayerDataInit();
        player->isComplete = true;
        auto newScene = MemoryScene::createScene("map/bglv1.png", "sound/endbg.mp3", "memory", true);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        isEndMap = true;
    }
    contactListener->isNext = false;
    SettingManager::getInstance()->loadSettingData();
}

void Boss4Scene::spawnObject() {
    Common::addMapName(this, "Chainbreaker");

    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Common::getPosition(x, map->getMapSize().height - y), bodyToSpriteMap);
                player->init(false);
                break;
            }
        }
    }

    // Spawn soul
    auto soulLayer = map->getLayer("soul");
    if (soulLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = soulLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    bossmap4 = new BossMap4(world, this, bodyToSpriteMap);
                    bossmap4->map = map;
                    bossmap4->player = player;
                    bossmap4->init(Common::getPosition(x, map->getMapSize().height - y));
                    break;
                }
            }
        }
    }
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}




