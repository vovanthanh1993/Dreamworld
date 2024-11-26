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
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = backStickLayer->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("Item/backstick/backstick2.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(0.35 * Common::scaleSizeXY());
                //sprite->setScaleX(2);
                sprite->setTag(Constants::TAG_STICK_ITEM);
                scene->addChild(sprite);
                auto rotateAction = RotateBy::create(2.0f, 360); // Thời gian 2 giây, góc 360 độ
                sprite->runAction(RepeatForever::create(rotateAction)); // Lặp lại mãi mãi

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;

                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 0.00001f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_ITEM;
                fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER;

                // Gán fixture cho body
                body->SetGravityScale(0.0f);
                body->CreateFixture(&fixtureDef);
                (*bodyToSpriteMap)[body] = sprite;
                Common::zoomAction(sprite);
            }
        }
    }
}

void MapItem::spawnChest() {
    auto wallLayerChest = map->getLayer("chest");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallLayerChest->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("Item/chest/closeChest.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(0.15 * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_CHEST);
                scene->addChild(sprite);

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;

                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_CHEST;
                fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_SLASH;// Constants::CATEGORY_PLAYER;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                (*bodyToSpriteMap)[body] = sprite;
            }
        }
    }
}

void MapItem::spawnHeart() {
    // spawn heart
    auto wallLayerHeart = map->getLayer("heart");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallLayerHeart->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("Item/gourd/heart.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(0.08 * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_HEART);
                scene->addChild(sprite);
                auto rotateAction = RotateBy::create(2.0f, 360); // Thời gian 2 giây, góc 360 độ
                sprite->runAction(RepeatForever::create(rotateAction)); // Lặp lại mãi mãi

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;

                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 0.00001f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_ITEM;
                fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_PLAYER;

                // Gán fixture cho body
                body->SetGravityScale(0.0f);
                body->CreateFixture(&fixtureDef);
                (*bodyToSpriteMap)[body] = sprite;
                Common::zoomAction(sprite);
            }
        }
    }
}

void MapItem::spawnBridge() {
    // spawn bridge
    auto wallBridge = map->getLayer("bridge");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallBridge->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("map/Bridge.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(Constants::BRIDGE_SCALE * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_BRIDGE);
                scene->addChild(sprite);
                int* userData = new int(-1);
                sprite->setUserData(userData);

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;


                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 1000.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
                fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT | Constants::CATEGORY_STICK;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                body->SetGravityScale(0.0f);
                (*bodyToSpriteMap)[body] = sprite;
                b2Vec2 velocity(0, Constants::SPEED_BRIDGE * Common::scaleSizeXY());
                body->SetLinearVelocity(velocity);
            }
        }
    }
}

void MapItem::spawnBridgeBreak() {
    // spawn bridge break
    auto wallBridgeBreak = map->getLayer("bridgebreak");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallBridgeBreak->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("map/BridgeBreak.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(Constants::BRIDGE_SCALE * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_BRIDGE_BREAK);
                scene->addChild(sprite);

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;


                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width - 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height - 40) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 1000000.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_WALL;
                fixtureDef.filter.maskBits = Constants::TAG_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                body->SetGravityScale(0.0f);
                (*bodyToSpriteMap)[body] = sprite;
            }
        }
    }
}

void MapItem::spawnBox() {
    auto box = map->getLayer("box");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = box->getTileAt(Vec2(x, y));
            if (tile) {
                auto sprite = Sprite::create("map/stone.png");
                sprite->setPosition(Vec2(origin.x / Common::scaleSizeXY() + x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE) * Common::scaleSizeXY());
                sprite->setScale(Constants::BOX_SCALE * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_BOX);
                scene->addChild(sprite, 0);
                int* userData = new int(-1);
                sprite->setUserData(userData);

                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;


                auto body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 500.0f;
                fixtureDef.friction = 1.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_BOX;
                fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT | Constants::CATEGORY_WALL;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                (*bodyToSpriteMap)[body] = sprite;
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
        fixtureDef.filter.maskBits = Constants::CATEGORY_NPC | Constants::CATEGORY_GEM | Constants::CATEGORY_ARROW | Constants::CATEGORY_STICK | Constants::CATEGORY_ENEMY | Constants::CATEGORY_PLAYER | Constants::CATEGORY_LIMIT | Constants::CATEGORY_CHEST | Constants::CATEGORY_BOX | Constants::CATEGORY_ITEM;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_WALL);
        body->SetUserData(sprite);
    }

    // Spawn limit
    auto limitLayer = map->getLayer("limit");
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
                fixtureDef.filter.maskBits = Constants::CATEGORY_NPC | Constants::CATEGORY_ENEMY | Constants::CATEGORY_WALL | Constants::CATEGORY_BOX;

                //// Gán fixture cho body
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    // Spawn limit map
    auto limitMapObject = map->getObjectGroup("limitmap");
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
        fixtureDef.filter.maskBits = Constants::CATEGORY_GEM | Constants::CATEGORY_ARROW | Constants::CATEGORY_STICK | Constants::CATEGORY_ENEMY | Constants::CATEGORY_PLAYER | Constants::CATEGORY_LIMIT | Constants::CATEGORY_CHEST | Constants::CATEGORY_BOX | Constants::CATEGORY_ITEM;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_PORT);
        body->SetUserData(sprite);
    }
}

void MapItem::spawnAll() {
    spawnWallAndLimit();
    spawnChest();
    spawnBackStick();
    spawnHeart();
    spawnBox();
    spawnBridge();
    spawnBridgeBreak();
    spawnEndGate();
}