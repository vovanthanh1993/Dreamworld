#include "Skull.h"
Skull::Skull(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, position, bodyToSpriteMap) {};

bool Skull::init() {
    sprite = Sprite::create("Enemy/Bossmap2/skull/skull.png");
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_SKULL);
    scene->addChild(sprite);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;
    bodyDef.bullet = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((((sprite->getContentSize().width) / 2-20) * sprite->getScale()) / Constants::PIXELS_PER_METER,
        (((sprite->getContentSize().height) / 2-20) * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.01f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 1.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ARROW;
    fixtureDef.filter.maskBits = Constants::CATEGORY_PLAYER | Constants::CATEGORY_WALL | Constants::CATEGORY_BLOCK;;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    return true;
}

void Skull::followPlayer() {
    // Lấy vị trí của enemy và player
    b2Vec2 playerPos = player->getBody()->GetPosition();
    b2Vec2 enemyPos = body->GetPosition();

    // Tính toán vector hướng từ enemy tới player
    b2Vec2 direction = playerPos - enemyPos;
    direction.Normalize();

    if (direction.x < 0) sprite->setScaleX(-scale);
    else sprite->setScaleX(scale);
    float speedAttack = 20.0f;  // Tốc độ di chuyển của enemy
    b2Vec2 velocity = Common::scaleSizeXY() * speedAttack * direction;
    body->SetLinearVelocity(velocity);
}