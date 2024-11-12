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
        float x = origin.x / Common::PIXELS_PER_METER + object["x"].asFloat() / Common::PIXELS_PER_METER * Common::scaleSprite();
        float y = object["y"].asFloat() / Common::PIXELS_PER_METER * Common::scaleSprite();
        float width = object["width"].asFloat() / Common::PIXELS_PER_METER * Common::scaleSprite();
        float height = object["height"].asFloat() / Common::PIXELS_PER_METER * Common::scaleSprite();

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
        fixtureDef.filter.categoryBits = Common::CATEGORY_WALL;
        fixtureDef.filter.maskBits = Common::CATEGORY_NPC | Common::CATEGORY_GEM | Common::CATEGORY_ARROW | Common::CATEGORY_STICK | Common::CATEGORY_ENEMY | Common::CATEGORY_PLAYER | Common::CATEGORY_LIMIT | Common::CATEGORY_CHEST | Common::CATEGORY_BOX | Common::CATEGORY_ITEM;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Common::TAG_WALL);
        body->SetUserData(sprite);
    }
}