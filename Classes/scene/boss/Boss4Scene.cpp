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
//on "init" you need to initialize your instance
bool Boss4Scene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }
    
    /*heartPool = new HeartPool(world, this, bodyToSpriteMap, 1);
    this->schedule([this](float dt) {
        spawnHeart();
        }, 10, "spawn_heart");

    backStickPool = new BackStickPool(world, this, bodyToSpriteMap, 1);
    this->schedule([this](float dt) {
        spawnBackStick();
        }, 8, "spawn_back_stick");*/
    return true;
}

// update
void Boss4Scene::update(float dt) {
    BaseScene::update(dt);

    if (bossmap4->isAlive) {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume() * 0.2);
    }

    if (contactListener->isNext && !bossmap4->isAlive) {
        player->savePlayerDataInit();
        auto newScene = EndScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        isEnd = true;
    }
    settingInit->loadSettingData();
}

void Boss4Scene::spawnObject() {

    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
                    bossmap4->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                    break;
                }
            }
        }
    }
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}

void Boss4Scene::spawnHeart() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int i = Common::randomNum(1, 4);
    int count = 0;
    auto wukongLayer = map->getLayer("heart");
    if (wukongLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wukongLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    if (i == ++count) {
                        Heart* w = heartPool->getFromPool();
                        if (w != nullptr) {
                            w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Boss4Scene::spawnBackStick() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int i = Common::randomNum(1, 4);
    int count = 0;
    auto wukongLayer = map->getLayer("backstick");
    if (wukongLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wukongLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    if (i == ++count) {
                        BackStick* w = backStickPool->getFromPool();
                        if (w != nullptr) {
                            w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                            break;
                        }
                    }
                }
            }
        }
    }
}




