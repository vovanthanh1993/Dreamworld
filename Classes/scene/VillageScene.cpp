#include "VillageScene.h"
#include "player/Player.h"
#include "item/Heart.h"
#include <string>
#include <vector>
#include "audio/include/AudioEngine.h"
#include "map/Wall.h"
#include "map/Limit.h"
#include "npc/NPC1.h"
#include "npc/DialogueBox.h"
#include "npc/ShopLayer.h"
#include "map/EndGate.h"
#include "Map1Scene.h"
#include "npc/NPCMonkey.h"


using namespace std;
USING_NS_CC;

Scene* VillageScene::createScene()
{
    return VillageScene::create();
}


//on "init" you need to initialize your instance
bool VillageScene::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
    int round = Common::loadRound();
    if (round == 1) {
        Common::showBossText(this, "It's that monkey again...");
    }
    else {
        Common::showBossText(this, "The monkey still needs to practice a lot more...");
    }
    settingInit->loadSettingData();
    settingInit->setBgMusicId(Common::playBackgroundMusic(settingInit->getVolume(), "sound/background2.mp3"));

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


    map = TMXTiledMap::create("map/village.tmx");
    map->setScale(Common::scaleSprite());
    map->setAnchorPoint(Vec2(0,0));
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    map->setPosition(origin);
    addChild(map, 0, 99);
    spawnObject();

   // Press
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(VillageScene::onKeyPressed, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // va cham
    contactListener =  new MyContactListener(player);
    contactListener->_bodyToSpriteMap = _bodyToSpriteMap;
    world->SetContactListener(contactListener);

    // Schedule the update method
    this->scheduleUpdate();

    return true;
}

// update
void VillageScene::update(float dt) {

    if (this->getChildByName("Menu") == nullptr) {
        isEnable = true;
    }
    if (shopLayer != nullptr) {
        isEnable = shopLayer->isEnable;
    }
    if (guiLayer != nullptr) {
        isEnable = guiLayer->isEnable;
    }
    if (skillShopLayer != nullptr) {
        isEnable = skillShopLayer->isEnable;
    }

    world->Step(dt, 8, 3); // Cập nhật thế giới Box2D
    player->isEnable = isEnable;
    player->updateMove();
    player->updateSlashVector(dt);

    // Đồng bộ hóa vị trí sprite với vị trí body
    Common::updatePosition(world, _bodyToSpriteMap);

    if (contactListener->isNext) {
        player->savePlayerDataInit();
        auto newScene = Map1Scene::create();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, newScene));
        contactListener->isNext = false;
    }
    
    settingInit->loadSettingData();
    AudioEngine::setVolume(settingInit->getBgMusicId(), settingInit->getVolume());
}

void VillageScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    if (isEnable) {
        if (keyCode == (EventKeyboard::KeyCode::KEY_E)) {
            if (b2Distance(player->getBody()->GetPosition(), npc1->getBody()->GetPosition()) <= Common::TALK_RANGE*Common::scaleSprite()) {
                shopLayer = ShopLayer::create();
                shopLayer->player = player;
                this->addChild(shopLayer, 5);
                player->getBody()->SetLinearVelocity(b2Vec2_zero);
                player->idle();
                player->isEnable = false;
            }

            if (b2Distance(player->getBody()->GetPosition(), npc2->getBody()->GetPosition()) <= Common::TALK_RANGE * Common::scaleSprite()) {
                guiLayer = GUILayer::create();
                guiLayer->player = player;
                this->addChild(guiLayer, 5);
                player->getBody()->SetLinearVelocity(b2Vec2_zero);
                player->idle();
                player->isEnable = false;
            }
            if (b2Distance(player->getBody()->GetPosition(), npc3->getBody()->GetPosition()) <= Common::TALK_RANGE * Common::scaleSprite()) {
                skillShopLayer = SkillShopLayer::create();
                skillShopLayer->player = player;
                this->addChild(skillShopLayer,5);
                player->getBody()->SetLinearVelocity(b2Vec2_zero);
                player->idle();
                player->isEnable = false;
            }
        }
    }
}

void VillageScene::spawnObject() {

    Wall* wall = new Wall(world, this, map);
    Limit* limit = new Limit(world, this, map);
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Spawn player
    auto playerLayer = map->getLayer("player");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = playerLayer->getTileAt(Vec2(x, y));
            if (tile) {
                player = new Player(world, this, Vec2(origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2, (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                player->init(true);
                player->isInVillage = true;
            }
        }
    }

    // Spawn player
    auto npc1Layer = map->getLayer("npc1");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc1Layer->getTileAt(Vec2(x, y));
            if (tile) {
    
                npc1 = new NPC1(world, this, Vec2((origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2), (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                npc1->startConversation(this);
            }
        }
    }

    // Spawn player
    auto npc3Layer = map->getLayer("npc3");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc3Layer->getTileAt(Vec2(x, y));
            if (tile) {

                npc3 = new NPC3(world, this, Vec2((origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2), (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                npc3->startConversation(this);
            }
        }
    }

    // Spawn player
    auto npc2Layer = map->getLayer("npc2");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npc2Layer->getTileAt(Vec2(x, y));
            if (tile) {

                npc2 = new NPC2(world, this, Vec2((origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2), (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
                npc2->startConversation(this);
            }
        }
    }

    // Spawn player
    auto npcMonkeyLayer = map->getLayer("npcmonkey");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = npcMonkeyLayer->getTileAt(Vec2(x, y));
            if (tile) {

                NPCMonkey* npcMonkey = new NPCMonkey(world, this, Vec2((origin.x + x * Common::TITLE_SIZE + Common::TITLE_SIZE / 2), (map->getMapSize().height - y) * Common::TITLE_SIZE) * Common::scaleSprite(), _bodyToSpriteMap);
            }
        }
    }
    EndGate* port = new EndGate(world, this, _bodyToSpriteMap, map);
    
}





