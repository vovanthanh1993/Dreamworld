#include "Port.h"
Port::Port(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Port::init() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto port = map->getLayer("port");
    for (int x = 0; x < map->getMapSize().width; ++x) {
        for (int y = 0; y < map->getMapSize().height; ++y) {
            auto tile = port->getTileAt(Vec2(x, y));
            if (tile) {
                
                auto spriteNode = SpriteBatchNode::create("map/port/sprites.png");
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile("map/port/sprites.plist");
                sprite = Sprite::createWithSpriteFrameName("Explosion_0.png");
                sprite->setPosition(Common::getPosition(x, map->getMapSize().height - y));
                sprite->setScale(Constants::PORT_SCALE * Common::scaleSizeXY());
                sprite->setTag(Constants::TAG_ENDGATE);
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
                fixtureDef.filter.categoryBits = Constants::CATEGORY_ITEM;
                fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER;

                // Gán fixture cho body
                body->CreateFixture(&fixtureDef);
                (*bodyToSpriteMap)[body] = sprite;
            }
        }
    }
    // Lên lịch gọi update mỗi frame
    scene->schedule([this](float dt) { this->update(dt); }, "Port");
    return true;
}

void Port::update(float dt) {
    Size screenSize = Director::getInstance()->getVisibleSize();
    if (sprite->getPosition().y <= screenSize.height/2) {
        body->SetLinearVelocity(b2Vec2_zero);
    }
}