#include "VillageScene.h"

using namespace std;
USING_NS_CC;

Scene* VillageScene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    VillageScene* ret = new VillageScene();
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
bool VillageScene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    if (!BaseScene::init(bg, bgMusic, mapName, isMoveCamera)) {
        return false;
    }

    // Press
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(VillageScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    return true;
}


// update
void VillageScene::update(float dt) {

    BaseScene::update(dt);

    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Map1Scene::createScene("map/bglv1.png", "sound/bg1.mp3", "map1", true);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
    }
}

void VillageScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (player->isEnable) {
        if (keyCode == (EventKeyboard::KeyCode::KEY_E)) {
            if (b2Distance(player->getBody()->GetPosition(), npc1->getBody()->GetPosition()) <= Constants::TALK_RANGE* Common::scaleSizeXY()) {
                upgradeShopLayer = UpgradeShopLayer::createLayer(player, this);
            }

            if (b2Distance(player->getBody()->GetPosition(), npc2->getBody()->GetPosition()) <= Constants::TALK_RANGE * Common::scaleSizeXY()) {
                guiLayer = GUILayer::createLayer(player, this);
            }
            if (b2Distance(player->getBody()->GetPosition(), npc3->getBody()->GetPosition()) <= Constants::TALK_RANGE * Common::scaleSizeXY()) {
                itemShopLayer = ItemShopLayer::createLayer(player, this);
            }
        }
    }
}

void VillageScene::spawnObject() {

    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();
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
    auto npc1Layer = map->getLayer("npc1");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc1Layer->getTileAt(Vec2(x, y));
            if (tile) {
                npc1 = new NPC1(world, this, bodyToSpriteMap);
                npc1->init(Vec2((origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2), (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }

    // Spawn player
    auto npc3Layer = map->getLayer("npc3");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc3Layer->getTileAt(Vec2(x, y));
            if (tile) {
                npc3 = new NPC3(world, this, bodyToSpriteMap);
                npc3->init(Vec2((origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2), (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }

    // Spawn player
    auto npc2Layer = map->getLayer("npc2");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc2Layer->getTileAt(Vec2(x, y));
            if (tile) {
                npc2 = new NPC2(world, this, bodyToSpriteMap);
                npc2->init(Vec2((origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2), (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }

    // Spawn player
    auto npcMonkeyLayer = map->getLayer("npcmonkey");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npcMonkeyLayer->getTileAt(Vec2(x, y));
            if (tile) {

                NPCMonkey* npcMonkey = new NPCMonkey(world, this, bodyToSpriteMap);
                npcMonkey->init(Vec2((origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2), (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
            }
        }
    }
}





