#include "Map3Scene.h"

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
    b2Vec2 gravity(0.0f, Constants::GRAVITY * Common::scaleSizeXY()); // Trọng lực theo chiều dương Y -400
    world = new b2World(gravity);
    

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create("map/bg2.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    background->setOpacity(128);
    Common::scaleAll(background, 1);
    this->addChild(background, 0);
    

    map = TMXTiledMap::create("map/map3.tmx");
    map->setScale(Common::scaleSizeXY());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener =  new MyContactListener(player, this, world);
    contactListener->bodyToSpriteMap = bodyToSpriteMap;
    contactListener->bossmap2 = bossmap2;
    world->SetContactListener(contactListener);
    

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void Map3Scene::update(float dt) {

    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D

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
        if (player->getSprite()->getPositionX() > screenWidth / 2 && player->getSprite()->getPositionX() < 14376 * Common::scaleSizeXY() - screenWidth / 2) {
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
    //// Cập nhật tất cả các enemyslash
    //for (auto it = slashEnemyVector.begin(); it != slashEnemyVector.end(); ) {
    //    (*it)->update(this);

    //    if (!(*it)->IsVisible()) {
    //        // Xóa sprite khỏi danh sách và bộ nhớ
    //        delete* it;
    //        it = slashEnemyVector.erase(it); // Loại bỏ sprite khỏi danh sách
    //    }
    //    else {
    //        ++it;
    //    }
    //}

    // New scene
    if (player->getSprite()->getPositionY() < 0) {
        cocos2d::AudioEngine::stopAll();
        auto newScene = Map3Scene::create();
        Director::getInstance()->replaceScene(newScene);

    }
    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = VillageScene::createScene("map/bglv1.png", "sound/background2.mp3", "village", false);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
        Common::saveRound(Common::loadRound()+1);
    }
    settingInit->loadSettingData();
    AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
}

void Map3Scene::spawnObject() {
    MapItem* item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnWallAndLimit();
    item->spawnEndGate();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // spawn acher
    auto layerElemental = map->getLayer("elemental");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = layerElemental->getTileAt(Vec2(x, y));
            if (tile) {
                Elemental* w = new Elemental(world, this, Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                w->init();
                elementalVector->push_back(w);
            }
        }
    }

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY(), bodyToSpriteMap);
                player->init(false);
            }
        }
    }
    Port* port = new Port(world, this, bodyToSpriteMap, map);
}





