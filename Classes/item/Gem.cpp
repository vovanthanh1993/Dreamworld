#include "Gem.h"
#include "player/Player.h"
Sprite* Gem::getSprite() {
    return sprite;
}

b2Body* Gem::getBody() {
    return body;
}
void Gem::init(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* _bodyToSpriteMap, int num) {
    
    int x = 0;
    if (num > 1) {
        x = -(num / 2);
    }
    for (int i = 0;i < num;i++) {
        sprite = Sprite::create("Item/gem/gem.png");
        sprite->setPosition(position);
        sprite->setScale(Common::GEM_SCALE * Common::scaleSprite());
        sprite->setTag(Common::TAG_GEM);
        scene->addChild(sprite);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
        bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER +x, sprite->getPositionY() / Common::PIXELS_PER_METER);
        bodyDef.fixedRotation = true;
        bodyDef.bullet = true;

        body = world->CreateBody(&bodyDef);
        body->SetUserData(sprite);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
            ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.05f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = Common::CATEGORY_GEM;
        fixtureDef.filter.maskBits = Common::CATEGORY_WALL | Common::CATEGORY_PLAYER;

        // Gán fixture cho body
        body->CreateFixture(&fixtureDef);
        (*_bodyToSpriteMap)[body] = sprite;
        b2Vec2 velocity(0, 50 * Common::scaleSprite());
        body->SetLinearVelocity(velocity);
        x += 1 * Common::scaleSprite();
    }
    
}