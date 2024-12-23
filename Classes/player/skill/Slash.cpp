#include "Slash.h"
#include "main/Effect.h"
Slash::Slash(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};

bool Slash::init(Vec2 position) {
    scale = 0.3;
    Effect::playerSlash();
    sprite = Sprite::create("Slash.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_SLASH);
    scene->addChild(sprite);

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
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_SLASH;
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY | Constants::CATEGORY_WALL | Constants::CATEGORY_CHEST | Constants::CATEGORY_ITEM;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);

    // Thoi gian khoi tao
    isActive = true;
    startTime = std::chrono::steady_clock::now();
    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "Slash");
    scene->addChild(this);

    return true;
}

void Slash::update(float dt) {
    if (isActive) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;

        if (elapsed.count() >= duration) {
            if (sprite != nullptr && body != nullptr) {
                isActive = false;
                world->DestroyBody(body);
                sprite->removeFromParentAndCleanup(true); // Xóa node Cocos2d
                this->removeFromParentAndCleanup(true);
            }
        }
    }
}