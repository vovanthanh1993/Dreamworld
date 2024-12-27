#include "MemoryScene.h"

using namespace std;
USING_NS_CC;

Scene* MemoryScene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    MemoryScene* ret = new MemoryScene();
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
void MemoryScene::update(float dt) {
    
    BaseScene::update(dt);

    if (player->getSprite()->getPositionY() < 0) {
        Director::getInstance()->replaceScene(MemoryScene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", true));
    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = EndScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        isEndMap = true;
    }
    contactListener->isNext = false;
}

void MemoryScene::spawnObject() {
    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Common::getPosition(x, map->getMapSize().height - y), bodyToSpriteMap);
                player->init(false);
                player->isInVillage = true;
                break;
            }
        }
    }

    // Spawn player
    int index = 1;
    auto memoryLayer = map->getLayer("memory");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = memoryLayer->getTileAt(Vec2(x, y));
            if (tile) {
                auto m = new Memory(world, this, bodyToSpriteMap);
                m->player = player;
                m->init(Common::getPosition(x, map->getMapSize().height - y), index++);
            }
        }
    }
    Port* p = new Port(world, this, bodyToSpriteMap);
    p->map = map;
    p->init();
    p->getSprite()->setScale(1.5);
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





