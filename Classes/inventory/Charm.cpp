#include "Charm.h"
Charm::Charm(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Charm::init(Vec2 position) {
    scale = 0.3;
    id = Common::randomNum(1, 6);
    manaBonus = Common::randomNum(-10, 10);
    healthBonus = Common::randomNum(-10, 10);
    dameBonus = Common::randomNum(-1 , 2);
    string manaBonusS = manaBonus >= 0 ? "+" + to_string(manaBonus) : to_string(manaBonus);
    string healthBonusS = healthBonus >= 0 ? "+" + to_string(healthBonus) : to_string(healthBonus);
    string dameBonusS = dameBonus >= 0 ? "+" + to_string(dameBonus) : to_string(dameBonus);
    effect = manaBonusS + " mana, " + healthBonusS + " health, " + dameBonusS + " damage";

    spritePath = "inventory/charm" + to_string(id) + ".png";
    sprite = Sprite::create(spritePath);
    sprite->setPosition(position);
    sprite->setScale(scale * Common::scaleSizeXY());
    sprite->setTag(Constants::TAG_EQUIPMENT);
    scene->addChild(sprite);
    sprite->setUserData(this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(position.x / Constants::PIXELS_PER_METER, position.y / Constants::PIXELS_PER_METER);
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
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ITEM;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL | Constants::CATEGORY_PLAYER;

    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);

    (*bodyToSpriteMap)[body] = sprite;
    scene->addChild(this);
    Common::zoomAction(sprite);
    return true;
}