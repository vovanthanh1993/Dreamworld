#include "Heart.h"
Heart::Heart(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map) : BaseItem(world, scene, _bodyToSpriteMap, map) {
};

void Heart::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap) {
    // spawn heart
    auto wallLayerHeart = map->getLayer("heart");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = wallLayerHeart->getTileAt(Vec2(x, y));
            if (tile) {
                sprite = Sprite::create("Item/gourd/heart.png");
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

                body = world->CreateBody(&bodyDef);
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
                (*_bodyToSpriteMap)[body] = sprite;
                Common::zoomAction(sprite);
            }
        }
    }
}