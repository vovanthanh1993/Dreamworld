#include "skill/SlashEnemy.h"
#include "main/Effect.h"

Sprite* SlashEnemy::getSprite() {
    return sprite;
}

b2Body* SlashEnemy::getBody() {
    return body;
}
void SlashEnemy::init(b2World* world, Scene* scene, Vec2 position) {
    Effect::playerSlash();
    sprite = Sprite::create("Slash.png");
    sprite->setPosition(position);
    sprite->setScale(Common::STICK_SCALE * Common::scaleSprite());
    sprite->setPosition(position);
    sprite->setTag(Common::TAG_SLASH_ENEMY);
    scene->addChild(sprite);

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Common::PIXELS_PER_METER, sprite->getPositionY() / Common::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Common::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Common::CATEGORY_SLASH_ENEMY;
    fixtureDef.filter.maskBits = Common::CATEGORY_PLAYER | Common::CATEGORY_WALL;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);

    // Thoi gian khoi tao
    visible = true;
    startTime = std::chrono::steady_clock::now();
}

void SlashEnemy::update(Scene* scene) {
    if (visible) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;

        if (elapsed.count() >= duration) {
            if (sprite != nullptr) {
                visible = false;
                // Xóa dữ liệu người dùng (Sprite) khỏi hệ thống đồ họa
                Sprite* s = static_cast<Sprite*>(body->GetUserData());
                body->GetWorld()->DestroyBody(body);
                // Xóa sprite khỏi thế giới Box2D
                sprite->removeFromParentAndCleanup(true);
            }

        }
    }
}

bool SlashEnemy::IsVisible() {
    return visible;
}