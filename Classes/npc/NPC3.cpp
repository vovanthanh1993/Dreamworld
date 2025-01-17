#include "NPC3.h"
NPC3::NPC3(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap) :BaseNode(world, scene, bodyToSpriteMap) {};

bool NPC3::init(Vec2 position) {
    scale = 0.5;
    spriteNode = SpriteBatchNode::create("NPC/NPC3/sprites.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("NPC/NPC3/sprites.plist");
    sprite = Sprite::createWithSpriteFrameName("Mater_Idle_0.png");
    sprite->setScale(scale * Common::scaleSizeXY());
   
    sprite->setUserData(this);
    sprite->setPosition(position);
    spriteNode->addChild(sprite);
    scene->addChild(spriteNode, 0);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Hoặc loại cơ thể phù hợp khác
    bodyDef.position.Set(sprite->getPositionX() / Constants::PIXELS_PER_METER, sprite->getPositionY() / Constants::PIXELS_PER_METER);
    bodyDef.fixedRotation = true;

    body = world->CreateBody(&bodyDef);
    body->SetUserData(sprite);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(((sprite->getContentSize().width) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER,
        ((sprite->getContentSize().height) / 2 * sprite->getScale()) / Constants::PIXELS_PER_METER); // Kích thước của hình dạng va chạm

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = Constants::CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = Constants::CATEGORY_WALL;
    // Gán fixture cho body
    body->CreateFixture(&fixtureDef);
    (*bodyToSpriteMap)[body] = sprite;
    sprite->setScaleX(-scale* Common::scaleSizeXY());
    idle();

    // Tạo nhãn tên
    auto nameLabel = Label::createWithTTF("Shop", "fonts/Marker Felt.ttf", 30 * Common::scaleSizeXY());
    nameLabel->setColor(Color3B::WHITE);
    nameLabel->setPosition(Vec2(sprite->getPositionX(), sprite->getPositionY() + (sprite->getContentSize().height / 2* scale + 15)*Common::scaleSizeY())); // Đặt tên trên đầu nhân vật
    scene->addChild(nameLabel);
    return true;
}
void NPC3::idle() {
    if (sprite != nullptr) {
        sprite->stopAllActions();
        auto animateW = Animate::create(Common::createAnimation("Mater_Idle_", 17, 0.1));
        animateW->retain();
        sprite->runAction(RepeatForever::create(animateW));
    } 
    
}