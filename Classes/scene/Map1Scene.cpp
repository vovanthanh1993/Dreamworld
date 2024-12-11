#include "Map1Scene.h"

using namespace std;
USING_NS_CC;

Scene* Map1Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Map1Scene* ret = new Map1Scene();
    if (ret && ret->init(bg, bgMusic, mapName, isMoveCamera)) {
        ret->autorelease();  // Tự động giải phóng bộ nhớ
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

////on "init" you need to initialize your instance
//bool Map1Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
//{
//    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
//        return false;
//    }
//    return true;
//}

// update
void Map1Scene::update(float dt) {
    
    BaseScene::update(dt);

    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        Director::getInstance()->replaceScene(Map1Scene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", true));
    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss1Scene::createScene("map/bglv1.png", "Enemy/Bossmap1/sound/bg.mp3", "boss1", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;
}

void Map1Scene::spawnObject() {
    BaseScene::spawnObject();
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // spawn acher
    auto layerAcher = map->getLayer("acher");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = layerAcher->getTileAt(Vec2(x, y));
            if (tile) {
                Acher* w = new Acher(world, this, bodyToSpriteMap);
                w->player = player;
                w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                
            }
        }
    }

    //Set enemy
    auto wallLayerEnemy = map->getLayer("warrior");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallLayerEnemy->getTileAt(Vec2(x, y));
            if (tile) {
                Warrior* w = new Warrior(world, this, bodyToSpriteMap);
                w->player = player;
                w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }
}





