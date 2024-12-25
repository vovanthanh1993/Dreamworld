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

// update
void Map3Scene::update(float dt) {

    BaseScene::update(dt);

    // New scene
    if (player->getSprite()->getPositionY() < 0) {
        auto newScene = Map3Scene::createScene("map/bg2.png", "sound/bg3.mp3", "map3", true);
        Director::getInstance()->replaceScene(newScene);

    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss3Scene::createScene("map/bg2.png", "Enemy/Bossmap3/sound/bg.mp3", "boss3", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        isEndMap = true;
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
                player = new Player(world, this, Common::getPosition(x, map->getMapSize().height - y), bodyToSpriteMap);
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
                Hedgehog* w = new Hedgehog(world, this, bodyToSpriteMap);
                w->player = player;
                w->init(Common::getPosition(x, map->getMapSize().height - y));
            }
        }
    }

    // spawn golem
    auto golemLayer = map->getLayer("golem");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = golemLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Golem* w = new Golem(world, this, bodyToSpriteMap);
                w->player = player;
                w->init(Common::getPosition(x, map->getMapSize().height - y));
            }
        }
    }

    // spawn bat
    auto batLayer = map->getLayer("bat");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = batLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Bat* w = new Bat(world, this, bodyToSpriteMap);
                w->player = player;
                w->init(Common::getPosition(x, map->getMapSize().height - y));
            }
        }
    }
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





