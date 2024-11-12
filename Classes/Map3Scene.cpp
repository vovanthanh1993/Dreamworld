#include "Map3Scene.h"
#include "player/Player.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "map/Port.h"
#include "enemy/Wraith.h"
#include "enemy/Elemental.h"
#include "VillageScene.h"
#include "item/MapItem.h"

using namespace std;
USING_NS_CC;

Scene* Map3Scene::createScene()
{
    
    return Map3Scene::create();
}

//on "init" you need to initialize your instance
bool Map3Scene::init()
{
    int round = Common::loadRound();
    if (round == 1) {
        Common::showBossText(this, "Do you remember the day you came to me?");
    }
    else if (round == 2) {
        Common::showBossText(this, "You want to gain the power to defeat your enemies...");
    }
    else if (round == 3) {
        Common::showBossText(this, "We had a pact, but you broke it...");
    }
    else if (round == 4) {
        Common::showBossText(this, "You killed Sun Wukong and all three others...");
    }
    else if (round == 5) {
        Common::showBossText(this, "I want the willing soul of Xuánzàng. Why did you kill him?");
    }
    else {
        Common::showBossText(this, "The gift I have for you will remain in this cycle of reincarnation forever.");
    }
    // Phát nhạc nền
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), "sound/background2.mp3"));
    
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
    b2Vec2 gravity(0.0f, Common::GRAVITY * Common::scaleSprite()); // Trọng lực theo chiều dương Y -400
    world = new b2World(gravity);
    

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/bg2.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    background->setOpacity(128);
    Common::scaleSprite(background, 1);
    this->addChild(background, 0);
    

    map = TMXTiledMap::create("map/map3.tmx");
    map->setScale(Common::scaleSprite());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener =  new MyContactListener(player, this, world);
    contactListener->_bodyToSpriteMap = _bodyToSpriteMap;
    contactListener->bossmap2 = bossmap2;
    contactListener->warriorVector = warriorVector;
    world->SetContactListener(contactListener);
    

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void Map3Scene::update(float dt) {

    // return game
    if (this->getChildByName("Menu") == nullptr) {
        isEnable = true;
    }
    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D

    for (size_t i = 0; i < warriorVector->size(); i++ /* no increment here */) {
        (*warriorVector)[i]->updateAttack(slashEnemyVector, player, dt);
    }

    for (size_t i = 0; i < elementalVector->size(); i++ /* no increment here */) {
        (*elementalVector)[i]->updateAttack(player, dt);
    }

    for (size_t i = 0; i < wraithVector->size(); i++ /* no increment here */) {
        (*wraithVector)[i]->updateAttack(player, dt);
    }

    player->updateMove();
    player->updateSlashVector(dt);
    //// Đồng bộ hóa vị trí sprite với vị trí body
    Common::updatePosition(world, _bodyToSpriteMap);

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
        if (player->getSprite()->getPositionX() > screenWidth / 2 && player->getSprite()->getPositionX() < 14376 * Common::scaleSprite() - screenWidth / 2) {
            Vec2 cameraPosition = player->getSprite()->getPosition() + origin;
            camera->setPosition3D(Vec3(cameraPosition.x, camera->getPositionY(), camera->getPosition3D().z));

            player->uiNode->setPositionX(cameraPosition.x - screenWidth / 2 - origin.x);
        }
    }

    // xoa cac vat the duoc danh dau
    contactListener->removeObject();

    // Dung yen khi van toc ==0
    if (player->getBody()->GetLinearVelocity().x == 0 && !_keysPressed.count(EventKeyboard::KeyCode::KEY_SPACE)
        && !_keysPressed.count(EventKeyboard::KeyCode::KEY_E)) {
        //player.idle();
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
        auto newScene = Map3Scene::create();
        Director::getInstance()->replaceScene(newScene);

    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = VillageScene::create();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
        Common::saveRound(Common::loadRound()+1);
    }
    settingInit->loadSettingData();
    AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
}

void Map3Scene::spawnObject() {
    MapItem* item = new MapItem(world, this, _bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto wallLayerChest = map->getLayer("chest");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wallLayerChest->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Chest* w = new Chest();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //        }
    //    }
    //}

    //// spawn heart
    //auto wallLayerHeart = map->getLayer("heart");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wallLayerHeart->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Heart* w = new Heart();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //            heartVector.push_back(w);
    //        }
    //    }
    //}

    // spawn acher
    auto layerElemental = map->getLayer("elemental");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = layerElemental->getTileAt(Vec2(x, y));
            if (tile) {
                Elemental* w = new Elemental(world, this, Vec2(origin.x / Common::scaleSprite() + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                w->init();
                elementalVector->push_back(w);
            }
        }
    }

    ////Set enemy
    //auto wallLayerEnemy = map->getLayer("warrior");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wallLayerEnemy->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Warrior* w = new Warrior();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //            w->walk();
    //            warriorVector->push_back(w);
    //        }
    //    }
    //}

    //// spawn bridge
    //auto wallBridge = map->getLayer("bridge");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wallBridge->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Bridge* w = new Bridge();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //        }
    //    }
    //}

    //// spawn bridge break
    //auto wallBridgeBreak = map->getLayer("bridgebreak");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wallBridgeBreak->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            BridgeBreak* w = new BridgeBreak();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //        }
    //    }
    //}

    //// spawn bridge break
    //auto box = map->getLayer("box");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = box->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Box* w = new Box();
    //            w->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //        }
    //    }
    //}

    //// spawn boss
    //auto bossLayer = map->getLayer("boss");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = bossLayer->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            bossmap2->init(world, this, Vec2(x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE), _bodyToSpriteMap);
    //            
    //        }
    //    }
    //}
    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                player->init(false);
            }
        }
    }

    //// spwan rain
    //auto wraithLayer = map->getLayer("Wraith");
    //for (int x = 0; x < map->getMapSize().width; ++x) {
    //    for (int y = 0; y < map->getMapSize().height; ++y) {
    //        auto tile = wraithLayer->getTileAt(Vec2(x, y));
    //        if (tile) {
    //            Wraith* w = new Wraith();
    //            w->init(world, this, Vec2(x* Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y)* Common::TITLE_SIZE), _bodyToSpriteMap);
    //            w->walk();
    //            wraithVector->push_back(w);
    //        }
    //    }
    //}
   
    Port* port = new Port(world, this, _bodyToSpriteMap, map);
}





