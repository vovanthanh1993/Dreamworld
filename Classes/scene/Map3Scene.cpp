#include "Map3Scene.h"
using namespace std;
USING_NS_CC;

Scene* Map3Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Map3Scene* ret = new Map3Scene();
    if (ret && ret->init(bg, bgMusic, mapName, isMoveCamera)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

////on "init" you need to initialize your instance
//bool Map2Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
//{
//    
//    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
//        return false;
//    }
//    return true;
//}

// update
void Map3Scene::update(float dt) {

    BaseScene::update(dt);

    // New scene
    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        auto newScene = Map3Scene::createScene("map/bg2.png", "sound/background2.mp3", "map3", true);
        Director::getInstance()->replaceScene(newScene);

    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss3Scene::createScene("map/bglv1.png", "Enemy/Bossmap2/sound/bg.mp3", "boss2", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;
}

void Map3Scene::spawnObject() {
    MapItem* item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnBase();
    item->spawnBridge(2);

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                player->init(false);
            }
        }
    }

    //// spawn acher
    //auto elementalLayer = map->getLayer("elemental");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = elementalLayer->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Elemental* w = new Elemental(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
    //            w->player = player;
    //            w->init();
    //        }
    //    }
    //}

    // spawn hedgehog
    auto hedgehogLayer = map->getLayer("hedgehog");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = hedgehogLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Hedgehog* w = new Hedgehog(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                w->player = player;
                w->init();
            }
        }
    }

    // spawn golem
    auto golemLayer = map->getLayer("golem");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = golemLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Golem* w = new Golem(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                w->player = player;
                w->init();
            }
        }
    }

    // spawn bat
    auto batLayer = map->getLayer("bat");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = batLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Bat* w = new Bat(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                w->player = player;
                w->init();
            }
        }
    }
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





