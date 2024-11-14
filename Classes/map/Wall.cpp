#include "Wall.h"

Sprite* Wall::getSprite() {
    return sprite;
}

b2Body* Wall::getBody() {
    return body;
}
Wall::Wall(b2World* world, Scene* scene, TMXTiledMap* map) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
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
}