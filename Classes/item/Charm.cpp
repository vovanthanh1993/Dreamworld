#include "Charm.h"
Charm::Charm(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};
bool Charm::init(Vec2 position) {
    scale = 0.3;
    id = Common::randomNum(1, 6);
    slashDamageBonus = Common::randomNum(-5 , 5);
    stickDamageBonus = Common::randomNum(-5, 5);
    eagleDamageBonus = Common::randomNum(-5, 5);
    getEffectString();

    spritePath = "charm/charm" + to_string(id) + ".png";
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

// Hàm để ghi đối tượng vào file
void Charm::saveToFile(ofstream& out) {
    
}

string Charm::getEffectString() {
    string slashDamageBonusS = slashDamageBonus >= 0 ? "+" + to_string(slashDamageBonus) : to_string(slashDamageBonus);
    string stickDamageBonusS = stickDamageBonus >= 0 ? "+" + to_string(stickDamageBonus) : to_string(stickDamageBonus);
    string eagleDamageBonusS = eagleDamageBonus >= 0 ? "+" + to_string(eagleDamageBonus) : to_string(eagleDamageBonus);
    return slashDamageBonusS + " slash dmg, " + stickDamageBonusS + " stick dmg, " + eagleDamageBonusS + " eagle dmg";
}