#include "BaseScene.h"

using namespace std;
USING_NS_CC;
BaseScene::BaseScene() {
    // Khởi tạo các thành phần riêng cho GameScene nếu cần
}

BaseScene::~BaseScene() {
    // Giải phóng tài nguyên của GameScene nếu cần
}

Scene* BaseScene::createScene(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    BaseScene* ret = new BaseScene();
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
bool BaseScene::init(string bg, string bgMusic, string mapName, bool isMoveCamera)
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    this->isMoveCamera = isMoveCamera;

    // Phát nhạc nền
    SettingManager::getInstance()->loadSettingData();
    MusicManager::getInstance()->playBackgroundMusic(bgMusic, true);
    MusicManager::getInstance()->setVolume(SettingManager::getInstance()->getVolume());

    // Tạo một Camera
    auto camera = Camera::create();
    camera->setCameraFlag(CameraFlag::USER1);
    this->addChild(camera);


    // Khởi tạo thế giới vật lý với trọng lực
    b2Vec2 gravity(0.0f, Constants::GRAVITY * Common::scaleSizeXY());
    world = new b2World(gravity);

    // Thêm lớp ảnh (hình nền)
    auto background = cocos2d::Sprite::create(bg);
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    background->setPosition(cocos2d::Vec2(0, 0));
    Common::scaleAll(background, 1);
    this->addChild(background, 0);

    map = TMXTiledMap::create("map/"+ mapName +".tmx");
    map->setScale(Common::scaleSizeXY());
    map->setAnchorPoint(Vec2(0, 0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

    // va cham
    contactListener = new MyContactListener(player, this, world);
    contactListener->bodyToSpriteMap = bodyToSpriteMap;
    world->SetContactListener(contactListener);


    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void BaseScene::update(float dt) {
    if (!player->isAlive || isEndMap) return;

    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D
    Common::updatePosition(world, bodyToSpriteMap);

    if (isMoveCamera) {
        // Lấy Camera
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        auto camera = this->getDefaultCamera();
        int x = camera->getPositionX();
        int y = camera->getPositionY();
        Size screenSize = Director::getInstance()->getVisibleSize();
        float screenWidth = screenSize.width;
        float screenHeight = screenSize.height;
        if (camera && player->getSprite())
        {
            // Cập nhật vị trí của Camera để theo dõi nhân vật
            if (player->getSprite()->getPositionX() > origin.x + screenWidth / 2 && player->getSprite()->getPositionX() < Constants::MAX_SIZE_MAP * Common::scaleSizeXY() - screenWidth / 2 + origin.x) {
                Vec2 cameraPosition = player->getSprite()->getPosition();
                camera->setPosition3D(Vec3(cameraPosition.x, camera->getPositionY(), camera->getPosition3D().z));
                boundaryBodyStart->SetTransform(b2Vec2((cameraPosition.x - origin.x - screenWidth / 2 - 140 * Common::scaleSizeXY()) / Constants::PIXELS_PER_METER, camera->getPositionY()), 0.0f);
                boundaryBodyEnd->SetTransform(b2Vec2((cameraPosition.x + origin.x + screenWidth / 2 + 140 * Common::scaleSizeXY()) / Constants::PIXELS_PER_METER, camera->getPositionY()), 0.0f);
            }
        }
    }
    
    // xoa cac vat the duoc danh dau
    contactListener->removeObject();
    SettingManager::getInstance()->loadSettingData();
    MusicManager::getInstance()->setVolume(SettingManager::getInstance()->getVolume());
}

void BaseScene::spawnObject() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    item = new MapItem(world, this, bodyToSpriteMap, map);
    item->spawnAll();

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

    // start and end
    boundaryBodyStart = Common::createBoundary(world, true);
    boundaryBodyEnd = Common::createBoundary(world, false);
}





