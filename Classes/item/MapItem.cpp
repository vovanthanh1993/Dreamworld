#include "MapItem.h"

MapItem::MapItem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map) {
    // Store the provided scene reference
    this->scene = scene;

    // Store the provided world reference
    this->world = world;

    // Store the reference to the body-to-sprite map
    this->bodyToSpriteMap = _bodyToSpriteMap;

    // Initialize the character's position
    this->map = map;
};

// spawn stick
void MapItem::spawnBackStick() {
    auto backStickLayer = map->getLayer("backstick");
    if (backStickLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = backStickLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    BackStick* item = new BackStick(world, scene, bodyToSpriteMap);
                    item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                }
            }
        }
    }
}

void MapItem::spawnChest() {
    auto wallLayerChest = map->getLayer("chest");
    if (wallLayerChest != nullptr) {
        if (wallLayerChest != nullptr) {
            for (int x = 0; x < map->getMapSize().width; ++x) {
                for (int y = 0; y < map->getMapSize().height; ++y) {
                    auto tile = wallLayerChest->getTileAt(Vec2(x, y));
                    if (tile) {
                        Chest* item = new Chest(world, scene, bodyToSpriteMap);
                        item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                    }
                }
            }
        }
    }
}

void MapItem::spawnHeart() {
    // spawn heart
    auto wallLayerHeart = map->getLayer("heart");
    if (wallLayerHeart != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wallLayerHeart->getTileAt(Vec2(x, y));
                if (tile) {
                    Heart* item = new Heart(world, scene, bodyToSpriteMap);
                    item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());

                }
            }
        }
    }
}

void MapItem::spawnBridge(int type) {
    // spawn bridge
    auto wallBridge = map->getLayer("bridge");
    if (wallBridge != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wallBridge->getTileAt(Vec2(x, y));
                if (tile) {
                    Bridge* item = new Bridge(world, scene, bodyToSpriteMap);
                    item->type = type;
                    item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                }
            }
        }
    }
}

void MapItem::spawnBrokenBridge() {
    // spawn bridge break
    auto wallBridgeBreak = map->getLayer("BrokenBridge");
    if (wallBridgeBreak != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = wallBridgeBreak->getTileAt(Vec2(x, y));
                if (tile) {
                    BrokenBridge* item = new BrokenBridge(world, scene, bodyToSpriteMap);
                    item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                }
            }
        }
    }
}

void MapItem::spawnStone() {
    auto stoneLayer = map->getLayer("stone");
    if (stoneLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = stoneLayer->getTileAt(Vec2(x, y));
                if (tile) {
                    Stone* item = new Stone(world, scene, bodyToSpriteMap);
                    item->init(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                }
            }
        }
    }
}

void MapItem::spawnWallAndLimit() {

    //Tạo PhysicsBody cho mỗi tile trong TMXTiledMap
    auto wallObject = map->getObjectGroup("wall");
    for (const auto& obj : wallObject->getObjects()) {
        auto object = obj.asValueMap();

        // Lấy thông tin đối tượng
        float x = origin.x / Constants::PIXELS_PER_METER + object["x"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float y = object["y"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float width = object["width"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float height = object["height"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();

        // Tạo body definition
        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody; // Hoặc b2_dynamicBody tùy vào yêu cầu
        bodyDef.position.Set(x + width / 2, y + height / 2);

        b2Body* body = world->CreateBody(&bodyDef);

        // Tạo shape
        b2PolygonShape boxShape;
        boxShape.SetAsBox(width / 2, height / 2);

        // Tạo fixture definition
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
        fixtureDef.filter.maskBits = Constants::CATEGORY_NPC | Constants::CATEGORY_GEM | Constants::CATEGORY_ARROW | Constants::CATEGORY_STICK | Constants::CATEGORY_ENEMY | Constants::CATEGORY_PLAYER | Constants::CATEGORY_LIMIT | Constants::CATEGORY_CHEST | Constants::CATEGORY_STONE | Constants::CATEGORY_ITEM;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_WALL);
        body->SetUserData(sprite);
    }

    // Spawn limit
    auto limitLayer = map->getLayer("limit");
    if (limitLayer != nullptr) {
        for (int x = 0; x < map->getMapSize().width; ++x) {
            for (int y = 0; y < map->getMapSize().height; ++y) {
                auto tile = limitLayer->getTileAt(Vec2(x, y));
                if (tile) {

                    // Tạo body definition
                    b2BodyDef bodyDef;
                    bodyDef.type = b2_kinematicBody; // Body tinh
                    bodyDef.position.Set(origin.x / Constants::PIXELS_PER_METER + tile->getPositionX() / Constants::PIXELS_PER_METER * Common::scaleSizeXY(), tile->getPositionY() / Constants::PIXELS_PER_METER * Common::scaleSizeXY());

                    b2Body* body = world->CreateBody(&bodyDef);
                    tile->setTag(Constants::TAG_LIMIT);
                    body->SetUserData(tile);

                    // Tạo shape definition
                    b2PolygonShape dynamicBox;
                    dynamicBox.SetAsBox((tile->getContentSize().width / 2) / Constants::PIXELS_PER_METER * Common::scaleSizeXY(),
                        (tile->getContentSize().height / 2) / Constants::PIXELS_PER_METER * Common::scaleSizeXY());

                    //// Tạo fixture definition
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &dynamicBox;
                    fixtureDef.density = 1.0f; // Mật độ của vật thể
                    fixtureDef.friction = 0.0f; // Ma sát
                    fixtureDef.restitution = 0; // Độ hồi phục (bouncing)
                    fixtureDef.filter.categoryBits = Constants::CATEGORY_LIMIT;
                    fixtureDef.filter.maskBits = Constants::CATEGORY_NPC | Constants::CATEGORY_ENEMY | Constants::CATEGORY_WALL | Constants::CATEGORY_STONE;

                    //// Gán fixture cho body
                    body->CreateFixture(&fixtureDef);
                }
            }
        }
    }
    

    // Spawn limit map
    auto limitMapObject = map->getObjectGroup("limitmap");
    if (limitMapObject != nullptr) {
        for (const auto& obj : limitMapObject->getObjects()) {
            auto object = obj.asValueMap();

            // Lấy thông tin đối tượng
            float x = origin.x / Constants::PIXELS_PER_METER + object["x"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
            float y = object["y"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
            float width = object["width"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
            float height = object["height"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();

            // Tạo body definition
            b2BodyDef bodyDef;
            bodyDef.type = b2_kinematicBody; // Hoặc b2_dynamicBody tùy vào yêu cầu
            bodyDef.position.Set(x + width / 2, y + height / 2);

            b2Body* body = world->CreateBody(&bodyDef);

            // Tạo shape
            b2PolygonShape boxShape;
            boxShape.SetAsBox(width / 2, height / 2);

            // Tạo fixture definition
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.0f;
            fixtureDef.restitution = 0.0f;
            fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
            fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER;

            body->CreateFixture(&fixtureDef);

            Sprite* sprite = new Sprite();
            sprite->setTag(Constants::TAG_LIMIT);
            body->SetUserData(sprite);
        }
    } 
}

void MapItem::spawnEndGate() {
    //Tạo PhysicsBody cho mỗi tile trong TMXTiledMap
    auto wallObject = map->getObjectGroup("endgate");
    for (const auto& obj : wallObject->getObjects()) {
        auto object = obj.asValueMap();

        // Lấy thông tin đối tượng
        float x = (origin.x / Common::scaleSizeXY() + object["x"].asFloat()) / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float y = object["y"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float width = object["width"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
        float height = object["height"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();

        // Tạo body definition
        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody; // Hoặc b2_dynamicBody tùy vào yêu cầu

        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        float posX = x + width / 2;
        if (posX < (origin.x + visibleSize.width) / Constants::PIXELS_PER_METER + width / 2) posX = (origin.x + visibleSize.width) / Constants::PIXELS_PER_METER + width / 2;
        bodyDef.position.Set(posX, y + height / 2);

        b2Body* body = world->CreateBody(&bodyDef);

        // Tạo shape
        b2PolygonShape boxShape;
        boxShape.SetAsBox(width / 2, height / 2);

        // Tạo fixture definition
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
        fixtureDef.filter.maskBits =  Constants::CATEGORY_PLAYER;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_ENDGATE);
        body->SetUserData(sprite);
    }
}

void MapItem::spawnAll() {
    spawnWallAndLimit();
    spawnChest();
    spawnBackStick();
    spawnHeart();
    spawnEndGate();
    spawnStone();
    spawnBridge(1);
    spawnBrokenBridge();
}

void MapItem::spawnBase() {
    spawnWallAndLimit();
    spawnChest();
    spawnBackStick();
    spawnHeart();
    spawnEndGate();
}