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
//on "init" you need to initialize your instance
bool MemoryScene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }

    
    // Schedule the spawnEnemies function every 5 seconds
    wukongPool = new WukongPool(world, this, bodyToSpriteMap, 5);
    this->schedule([this](float dt) {
        spawnWukong();
        }, 3, "spawn_wukong");

    // Pool for bat
    batPool = new BatPool(world, this, bodyToSpriteMap, 10);
    this->schedule([this](float dt) {
        spawnBat();
        }, 5, "spawn_bat");
    return true;
}

// update
void MemoryScene::update(float dt) {

    BaseScene::update(dt);

    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Map1Scene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
    }
}

void MemoryScene::spawnObject() {

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
}
void MemoryScene::spawnBat() {
    // spawn bat
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int i = Common::randomNum(1, 4);
    int count = 0;
    auto batLayer = map->getLayer("bat");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = batLayer->getTileAt(Vec2(x, y));
            if (tile) {
                if (i == ++count) {
                    Bat* w = batPool->getFromPool();
                    if (w != nullptr) {
                        w->player = player;
                        w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                        w->attackRange = 1000;

                        // Lặp qua tất cả các fixture của body
                        for (b2Fixture* fixture = w->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                            // Lấy dữ liệu bộ lọc hiện tại
                            b2Filter filter = fixture->GetFilterData();

                            // Cập nhật categoryBits và maskBits
                            filter.categoryBits = Constants::CATEGORY_ENEMY;
                            filter.maskBits = Constants::CATEGORY_STICK | Constants::CATEGORY_SLASH | Constants::CATEGORY_PLAYER;

                            // Thiết lập lại dữ liệu bộ lọc
                            fixture->SetFilterData(filter);
                        }
                        break;
                    }
                }
            }
        }
    }
}

void MemoryScene::spawnWukong() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int i = Common::randomNum(1, 4);
    int count = 0;
    auto wukongLayer = map->getLayer("wukong");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wukongLayer->getTileAt(Vec2(x, y));
            if (tile) {
                if (i == ++count) {
                    Wukong* w = wukongPool->getFromPool();
                    if (w != nullptr) {
                        w->player = player;
                        w->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                        break;
                    }
                }
            }
        }
    }
}





