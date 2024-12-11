#include "MikoScene.h"

using namespace std;
USING_NS_CC;

Scene* MikoScene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    MikoScene* ret = new MikoScene();
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
bool MikoScene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }

    // Press
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(MikoScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    return true;
}


// update
void MikoScene::update(float dt) {

    BaseScene::update(dt);

    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Map1Scene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", true);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
    }
}

void MikoScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (player->isEnable) {
        if (keyCode == (EventKeyboard::KeyCode::KEY_E)) {
            if (b2Distance(player->getBody()->GetPosition(), miko->getBody()->GetPosition()) <= Constants::TALK_RANGE* Common::scaleSizeXY()) {
                //shopLayer = ShopLayer::createLayer(player, this);
            }
        }
    }
}

void MikoScene::spawnObject() {

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
                player->init(true);
                player->isInVillage = true;
                break;
            }
        }
    }

    // Spawn player
    auto mikoLayer = map->getLayer("miko");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = mikoLayer->getTileAt(Vec2(x, y));
            if (tile) {
                miko = new Miko(world, this, bodyToSpriteMap);
                miko->init(Vec2(origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());

                break;
            }
        }
    }

    // Spawn player
    auto npc1Layer = map->getLayer("npc1");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc1Layer->getTileAt(Vec2(x, y));
            if (tile) {
                NPC1* npc1 = new NPC1(world, this, bodyToSpriteMap);
                npc1->init(Vec2((origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2), (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }
}





