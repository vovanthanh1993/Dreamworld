#include "Slash.h"
#include "main/Effect.h"
Slash::Slash(b2World* world, Scene* scene, Vec2 position) :BaseNode(world, scene, position) {};

bool Slash::init() {
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
    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "Slash");
    scene->addChild(this);

    return true;
}

void Slash::update(float dt) {
    if (visible) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;

        if (elapsed.count() >= duration) {
            if (sprite != nullptr && body != nullptr) {
                visible = false;
                // Hủy body Box2D nếu nó tồn tại
                if (body)
                {
                    world->DestroyBody(body);
                    body = nullptr; // Đảm bảo body không còn trỏ tới bất kỳ bộ nhớ nào
                }

                // Xóa sprite nếu nó tồn tại
                if (sprite)
                {
                    sprite->removeFromParentAndCleanup(true); // Xóa node Cocos2d
                    sprite = nullptr; // Đảm bảo sprite không còn trỏ tới bất kỳ bộ nhớ nào
                }
            }
        }
    }
}