#include "Boss2Scene.h"
#include "player/Player.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "enemy/Wraith.h"
#include "scene/Map3Scene.h"
#include "item/MapItem.h"

using namespace std;
USING_NS_CC;

Scene* Boss2Scene::createScene()
{
    return Boss2Scene::create();
}

//on "init" you need to initialize your instance
bool Boss2Scene::init()
{
    
    // Phát nhạc nền
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), "Enemy/Bossmap2/sound/bg.mp3"));
    
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // Tạo một Camera
    auto camera = Camera::create();
    camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(camera);
   

    // Khởi tạo thế giới vật lý với trọng lực
    b2Vec2 gravity(0.0f, Constants::GRAVITY* Common::scaleSizeXY()); // Trọng lực theo chiều dương Y -400
    world = new b2World(gravity);
    

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/bglv1.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    map = TMXTiledMap::create("map/boss2.tmx");
    map->setScale(Common::scaleSizeXY());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener =  new MyContactListener(player, this, world);
    contactListener->_bodyToSpriteMap = _bodyToSpriteMap;
    contactListener->bossmap2 = bossmap2;
    world->SetContactListener(contactListener);
    

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void Boss2Scene::update(float dt) {
    if (!player->isAlive) return;

    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D

    if (bossmap2->isALive) {
        bossmap2->updateAttack(player, dt, map);
        bossmap2->updateHealthBarPosition();
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume()*0.2);
    }
    player->updateMove();
    player->updateSlashVector(dt);

    //// Đồng bộ hóa vị trí sprite với vị trí body
    Common::updatePosition(world, _bodyToSpriteMap);

    // xoa cac vat the duoc danh dau
    contactListener->removeObject();

    //-------------------CAP NHAT LAI SPRITE--------------------------
    if (contactListener->isNext && !bossmap2->isALive) {
        player->savePlayerDataInit();
        auto newScene = Map3Scene::create();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        
    }
    contactListener->isNext = false;
    settingInit->loadSettingData();
}

void Boss2Scene::spawnObject() {
    MapItem* item = new MapItem(world, this, _bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();

    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x / Common::scaleSizeXY()+x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE)* Common::scaleSizeXY(), _bodyToSpriteMap);
                player->init(false);
            }
        }
    }

    // spawn boss
    auto bossLayer = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = bossLayer->getTileAt(Vec2(x, y));
            if (tile) {
                bossmap2 = new BossMap2(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), _bodyToSpriteMap);
                bossmap2->init();
            }
        }
    }
}





