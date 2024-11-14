#include "Slash.h"
#include "main/Effect.h"
Sprite* Slash::getSprite() {
    return sprite;
}

b2Body* Slash::getBody() {
    return body;
}
void Slash::init(b2World* world, Scene* scene, Vec2 position) {
    Effect::playerSlash();
    sprite = Sprite::create("Slash.png");
    sprite->setPosition(position);
    sprite->setScale(Constants::STICK_SCALE * Common::scaleSizeXY());
    sprite->setPosition(position);
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
    fixtureDef.filter.maskBits = Constants::CATEGORY_ENEMY | Constants::CATEGORY_WALL | Constants::CATEGORY_CHEST;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);

    // Thoi gian khoi tao
    visible = true;
    startTime = std::chrono::steady_clock::now();
}

void Slash::update(float deltaTime, Scene* scene) {
    if (visible) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;

        if (elapsed.count() >= duration) {
            if (sprite != nullptr && body != nullptr) {
                visible = false;


                body->GetWorld()->DestroyBody(body);
                // Xóa sprite khỏi thế giới Box2D
                sprite->removeFromParentAndCleanup(true);
            }
        }
    }
}

bool Slash::IsVisible() {
    return visible;
}