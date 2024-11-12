#include "Boss1Scene.h"
#include "player/Player.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "Map2Scene.h"

using namespace std;
USING_NS_CC;
Scene* Boss1Scene::createScene()
{
    
    return Boss1Scene::create();
}

//on "init" you need to initialize your instance
bool Boss1Scene::init()
{
    // Phát nhạc nền
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(),"Enemy/Bossmap1/sound/bg.mp3"));
    
    
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    //cocos2d::AudioEngine::preload("sound/ting.wav");

    // Tạo một Camera
    auto camera = Camera::create();
    camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(camera);
   

    // Khởi tạo thế giới vật lý với trọng lực
    b2Vec2 gravity(0.0f, Common::GRAVITY* Common::scaleSprite()); // Trọng lực theo chiều dương Y -400
    world = new b2World(gravity);
    

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/bglv1.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleSprite(background, 1);
    this->addChild(background, 0);

    map = TMXTiledMap::create("map/boss1map.tmx");
    map->setScale(Common::scaleSprite());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener =  new MyContactListener(player, this, world);
    contactListener->_bodyToSpriteMap = _bodyToSpriteMap;
    contactListener->bossmap1 = bossmap1;
    world->SetContactListener(contactListener);
    

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void Boss1Scene::update(float dt) {
    if (!player->isAlive) return;

    if (this->getChildByName("Menu") == nullptr) {
        isEnable = true;
    }

    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D

    player->updateMove();
    player->updateSlashVector(dt);
    //// Đồng bộ hóa vị trí sprite với vị trí body
    Common::updatePosition(world, _bodyToSpriteMap);

    // xoa cac vat the duoc danh dau
    contactListener->removeObject();

    //-------------------CAP NHAT LAI SPRITE--------------------------
    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        Director::getInstance()->replaceScene(Boss1Scene::createScene());
    }
    
    // Boss map 1
    if (bossmap1->isALive) {
        bossmap1->updateAttack(player, dt);
        bossmap1->updateHealthBarPosition();
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
    }
    else {
        AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume() * 0.2);
    }
    if (contactListener->isNext && !bossmap1->isALive) {
        player->savePlayerDataInit();
        auto newScene = Map2Scene::create();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;
    settingInit->loadSettingData();
    
}

void Boss1Scene::spawnObject() {
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
                player = new Player(world, this, Vec2(origin.x / Common::scaleSprite() +x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE)* Common::scaleSprite(), _bodyToSpriteMap);
                player->init(false);
                break;
            }
        }
    }

    // spawn boss
    auto bossLayer = map->getLayer("boss");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = bossLayer->getTileAt(Vec2(x, y));
            if (tile) {
                bossmap1 = new BossMap1(world, this, Vec2(origin.x / Common::scaleSprite() + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                bossmap1->init();
                break;
            }
        }
    }
}





