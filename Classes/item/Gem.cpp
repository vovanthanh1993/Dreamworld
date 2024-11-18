#include "Gem.h"
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
        sprite->setScale(Constants::GEM_SCALE * Common::scaleSizeXY());
        sprite->setTag(Constants::TAG_GEM);
        scene->addChild(sprite);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
        bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER +x, sprite->getPositionY() / Constants::PIXELS_PER_METER);
        bodyDef.fixedRotation = true;
        bodyDef.bullet = true;

        body = world->CreateBody(&bodyDef);
        body->SetUserData(sprite);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
            ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.05f;
        fixtureDef.friction = 0.0f;
        fixtureDef.restitution = 0.0f;
        fixtureDef.filter.categoryBits = Constants::CATEGORY_GEM;
        fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_PLAYER;

        // Gán fixture cho body
        body->CreateFixture(&fixtureDef);
        (*_bodyToSpriteMap)[body] = sprite;
        b2Vec2 velocity(0, 50 * Common::scaleSizeXY());
        body->SetLinearVelocity(velocity);
        x += 1 * Common::scaleSizeXY();
    }
    
}