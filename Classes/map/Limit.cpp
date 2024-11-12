#include "Limit.h"

Sprite* Limit::getSprite() {
    return sprite;
}

b2Body* Limit::getBody() {
    return body;
}
Limit::Limit(b2World* world, Scene* scene, TMXTiledMap* map) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // limit 
    auto limitLayer = map->getLayer("limit");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = limitLayer->getTileAt(Vec2(x, y));
            if (tile) {

                // Tạo body definition
                b2BodyDef bodyDef;
                bodyDef.type = b2_kinematicBody; // Body tinh
                bodyDef.position.Set(origin.x / Common::PIXELS_PER_METER +tile->getPositionX() / Common::PIXELS_PER_METER * Common::scaleSprite(), tile->getPositionY() / Common::PIXELS_PER_METER * Common::scaleSprite());

                b2Body* body = world->CreateBody(&bodyDef);
                tile->setTag(Common::TAG_LIMIT);
                body->SetUserData(tile);

                // Tạo shape definition
                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox((tile->getContentSize().width / 2) / Common::PIXELS_PER_METER * Common::scaleSprite(),
                    (tile->getContentSize().height / 2) / Common::PIXELS_PER_METER * Common::scaleSprite());

                //// Tạo fixture definition
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 1.0f; // Mật độ của vật thể
                fixtureDef.friction = 0.0f; // Ma sát
                fixtureDef.restitution = 0; // Độ hồi phục (bouncing)
                fixtureDef.filter.categoryBits = Common::CATEGORY_LIMIT;
                fixtureDef.filter.maskBits = Common::CATEGORY_NPC | Common::CATEGORY_ENEMY | Common::CATEGORY_WALL | Common::CATEGORY_BOX;

                //// Gán fixture cho body
                body->CreateFixture(&fixtureDef);
            }
        }
    }
}