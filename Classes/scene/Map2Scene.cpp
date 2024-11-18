#include "scene/Map2Scene.h"
#include "player/Player.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "scene/boss/Boss2Scene.h"

using namespace std;
USING_NS_CC;

Scene* Map2Scene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    Map2Scene* ret = new Map2Scene();
    if (ret && ret->init(bg, bgMusic, mapName, isMoveCamera)) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

//on "init" you need to initialize your instance
bool Map2Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }
    contactListener->acherVector = acherVector;
    contactListener->warriorVector = warriorVector;
    world->SetContactListener(contactListener);
    return true;
}

// update
void Map2Scene::update(float dt) {
    BaseScene::update(dt);

    for (size_t i = 0; i < warriorVector->size(); i++ /* no increment here */) {
        (*warriorVector)[i]->updateAttack(slashEnemyVector, player, dt);
    }

    for (size_t i = 0; i < acherVector->size(); i++ /* no increment here */) {
        (*acherVector)[i]->updateAttack(player, dt);
    }

    for (size_t i = 0; i < wraithVector->size(); i++ /* no increment here */) {
        (*wraithVector)[i]->updateAttack(player, dt);
    }

    //-------------------CAP NHAT LAI SPRITE--------------------------
    // Cập nhật tất cả các enemyslash
    for (auto it = slashEnemyVector.begin(); it != slashEnemyVector.end(); ) {
        (*it)->update(this);

        if (!(*it)->IsVisible()) {
            // Xóa sprite khỏi danh sách và bộ nhớ
            delete* it;
            it = slashEnemyVector.erase(it); // Loại bỏ sprite khỏi danh sách
        }
        else {
            ++it;
        }
    }

    // New scene
    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        auto newScene = Map2Scene::createScene("map/bglv1.png", "sound/background2.mp3", "map2", true);
        Director::getInstance()->replaceScene(newScene);

    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss2Scene::createScene("map/bglv1.png", "Enemy/Bossmap2/sound/bg.mp3", "boss2", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;
}

void Map2Scene::spawnObject() {
    BaseScene::spawnObject();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // spawn acher
    auto layerAcher = map->getLayer("acher");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = layerAcher->getTileAt(Vec2(x, y));
            if (tile) {
                Acher* w = new Acher(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), _bodyToSpriteMap);
                w->init();
                acherVector->push_back(w);
            }
        }
    }

    //Set enemy
    auto wallLayerEnemy = map->getLayer("warrior");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallLayerEnemy->getTileAt(Vec2(x, y));
            if (tile) {
                Warrior* w = new Warrior(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), _bodyToSpriteMap);
                w->init();
                w->walk();
                warriorVector->push_back(w);
            }
        }
    }
    // spwan rain
    auto wraithLayer = map->getLayer("Wraith");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wraithLayer->getTileAt(Vec2(x, y));
            if (tile) {
                Wraith* w = new Wraith(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), _bodyToSpriteMap);
                w->init();
                w->walk();
                wraithVector->push_back(w);
            }
        }
    }
}





