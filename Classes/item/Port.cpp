#include "Port.h"

Sprite* Port::getSprite() {
    return sprite;
}

b2Body* Port::getBody() {
    return body;
}
Port::Port(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, TMXTiledMap* map) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto box = map->getLayer("port");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = box->getTileAt(Vec2(x, y));
            if (tile) {
                
                auto spriteNode = SpriteBatchNode::create("map/port/sprites.png");
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map/port/sprites.plist");
                sprite = Sprite::createWithSpriteFrameName("Explosion_0.png");
                sprite->setPosition(Vec2(origin.x+x * Constants::TITLE_SIZE + Constants::TITLE_SIZE / 2, (map->getMapSize().height - y) * Constants::TITLE_SIZE));
                sprite->setScale(Constants::PORT_SCALE * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_PORT);
                spriteNode->addChild(sprite);
                scene->addChild(spriteNode, 1);
                auto animateW = Animate::create(Common::createAnimation("Explosion_", 4, 0.2));
                animateW->retain();
                sprite->runAction(RepeatForever::create(animateW));

                b2BodyDef bodyDef;
                bodyDef.type = b2_kinematicBody; // Hoặc loại cơ thể phù hợp khác
                bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
                bodyDef.fixedRotation = true;
                bodyDef.bullet = true;


                body = world->CreateBody(&bodyDef);
                body->SetUserData(sprite);

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox(((sprite->getContentSize().width - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
                    ((sprite->getContentSize().height - 20) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicBox;
                fixtureDef.density = 20.0f;
                fixtureDef.friction = 0.0f;
                fixtureDef.restitution = 0.0f;
                fixtureDef.filter.categoryBits = Constants::CATEGORY_BOX;
                fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_ARROW | Constants::CATEGORY_LIMIT | Constants::CATEGORY_WALL;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                (*_bodyToSpriteMap)[body] = sprite;
            }
        }
    }
}