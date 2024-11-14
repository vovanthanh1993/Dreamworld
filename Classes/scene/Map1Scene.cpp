#include "Map1Scene.h"
#include "player/Player.h"
#include "item/Heart.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "map/Wall.h"
#include "map/Limit.h"
#include "scene/Map2Scene.h"
#include "map/EndGate.h"
#include "map/SizeLimit.h"
#include "scene/boss/Boss1Scene.h"


using namespace std;
USING_NS_CC;
Scene* Map1Scene::createScene()
{
    
    return Map1Scene::create();
}

//on "init" you need to initialize your instance
bool Map1Scene::init()
{
    // Phát nhạc nền
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), "sound/background2.mp3"));
    
    
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
    b2Vec2 gravity(0.0f, Constants::GRAVITY * Common::scaleSizeXY()); // Trọng lực theo chiều dương Y -400
    world = new b2World(gravity);
    

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/bglv1.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    map = TMXTiledMap::create("map/map1.tmx");
    map->setScale(Common::scaleSizeXY());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener =  new MyContactListener(player, this, world);
    contactListener->_bodyToSpriteMap = _bodyToSpriteMap;
    contactListener->acherVector = acherVector;
    contactListener->warriorVector = warriorVector;
    world->SetContactListener(contactListener);
    

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void Map1Scene::update(float dt) {
    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D
   
    for (size_t i = 0; i < warriorVector->size();i++ /* no increment here */) {
        (*warriorVector)[i]->updateAttack(slashEnemyVector, player, dt);
    }

    for (size_t i = 0; i < acherVector->size();i++ /* no increment here */) {
        (*acherVector)[i]->updateAttack(player, dt);
    }

    //// Đồng bộ hóa vị trí sprite với vị trí body
    Common::updatePosition(world, _bodyToSpriteMap);

    player->updateMove();
    player->updateSlashVector(dt);
    // Lấy Camera
    auto camera = this->getDefaultCamera();
    int x = camera->getPositionX();
    int y = camera->getPositionY();
    x = player->uiNode->getPositionX();
    y = player->uiNode->getPositionY();
    Size screenSize = Director::getInstance()->getVisibleSize();
    
    float screenWidth = screenSize.width;
    float screenHeight = screenSize.height;

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (camera && player->getSprite())
    {
        // Cập nhật vị trí của Camera để theo dõi nhân vật
        // Bạn có thể thêm một khoảng offset để điều chỉnh
        if (player->getSprite()->getPositionX() > origin.x+screenWidth / 2 && player->getSprite()->getPositionX() < 14376* Common::scaleSizeXY() - screenWidth / 2+ origin.x) {
            Vec2 cameraPosition = player->getSprite()->getPosition();
            camera->setPosition3D(Vec3(cameraPosition.x, camera->getPositionY(), camera->getPosition3D().z));

            player->uiNode->setPositionX(cameraPosition.x - screenWidth / 2- origin.x);
            boundaryBodyStart->SetTransform(b2Vec2((cameraPosition.x- origin.x - screenWidth / 2-120* Common::scaleSizeXY()) / Constants::PIXELS_PER_METER, camera->getPositionY()), 0.0f);
            boundaryBodyEnd->SetTransform(b2Vec2((cameraPosition.x+ origin.x+ screenWidth/2+120 * Common::scaleSizeXY())/Constants::PIXELS_PER_METER, camera->getPositionY()), 0.0f);
        }
    }
    // xoa cac vat the duoc danh dau
    contactListener->removeObject();


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

    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        Director::getInstance()->replaceScene(Map1Scene::createScene());
    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Boss1Scene::create();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
    }
    contactListener->isNext = false;

    settingInit->loadSettingData();
    AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
}

void Map1Scene::spawnObject() {
    MapItem* item = new MapItem(world, this, _bodyToSpriteMap, map);
    item->spawnAll();
    
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

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), _bodyToSpriteMap);
                player->init(false);
            }
        }
    }
    
    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





