#include "skill/StoneBall.h"
#include "main/Effect.h"
StoneBall::StoneBall(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {};

bool StoneBall::init() {
    sprite = Sprite::create("Enemy/Bossmap1/stoneball/stoneball.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setPosition(position);
    sprite->setTag(Constants::TAG_STONE_BALL);
    scene->addChild(sprite);
    sprite->setUserData(this);

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
    fixtureDef.restitution = 1.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_WALL;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    // Thoi gian khoi tao
    isActive = true;
    startTime = std::chrono::steady_clock::now();

    // Lên lịch gọi update mỗi frame
    this->schedule([this](float dt) { this->update(dt); }, "StoneBall");
    scene->addChild(this);

    return true;
}

void StoneBall::update(float dt) {
    if (isActive) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;
        if (elapsed.count() >= duration) {
            {
                BaseNode::destroyNode();
            }
        }
    }
}

void StoneBall::destroy() {
    isActive = false;
    //sprite->stopAllActions();
    //bodyToSpriteMap->erase(body);

    //// Hủy body Box2D nếu nó tồn tại
    //if (body)
    //{
    //    world->DestroyBody(body);
    //    body = nullptr; // Đảm bảo body không còn trỏ tới bất kỳ bộ nhớ nào
    //}

    //// Xóa sprite nếu nó tồn tại
    //if (sprite)
    //{
    //    sprite->removeFromParentAndCleanup(true); // Xóa node Cocos2d
    //    sprite = nullptr; // Đảm bảo sprite không còn trỏ tới bất kỳ bộ nhớ nào
    //}
}