#include "SizeLimit.h"

Sprite* SizeLimit::getSprite() {
    return sprite;
}

b2Body* SizeLimit::getBody() {
    return body;
}
SizeLimit::SizeLimit(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map, string layerName) {

    //Tạo PhysicsBody cho mỗi tile trong TMXTiledMap
    auto wallObject = map->getObjectGroup(layerName);
    for (const auto& obj : wallObject->getObjects()) {
        auto object = obj.asValueMap();

        // Lấy thông tin đối tượng
        float x = object["x"].asFloat() / Constants::PIXELS_PER_METER * Common::scaleSizeXY();
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
        fixtureDef.filter.maskBits = Constants::CATEGORY_ARROW | Constants::CATEGORY_STICK;

        body->CreateFixture(&fixtureDef);

        Sprite* sprite = new Sprite();
        sprite->setTag(Constants::TAG_PORT);
        body->SetUserData(sprite);
    }
}