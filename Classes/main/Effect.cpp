#include "Effect.h"
namespace common {
    void Effect::destroyStick(b2World* world, Scene* scene, Vec2 position) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/stick/sprites.plist");

        auto spriteNode = SpriteBatchNode::create("Effect/stick/sprites.png");
        auto sprite = Sprite::createWithSpriteFrameName("t0.png");

        sprite->setPosition(position);
        sprite->setScale(Common::scaleSizeXY());
        auto animate = Animate::create(Common::createAnimation("t", 6, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);

    }


    void Effect::destroyArrow(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/arrow/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/arrow/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("b0.png");
        sprite->setPosition(position);
        sprite->setScale(Common::scaleSizeXY());
        auto animate = Animate::create(Common::createAnimation("b", 8, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }

    void Effect::destroyRain(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/rain/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/rain/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("0.png");
        sprite->setPosition(position);
        sprite->setScale(Common::scaleSizeXY());
        auto animate = Animate::create(Common::createAnimation("", 11, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }

    void Effect::healing(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/healing/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/healing/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("healing_effect 0.png");
        sprite->setPosition(position);
        sprite->setScale(Common::scaleSizeXY());
        auto animate = Animate::create(Common::createAnimation("healing_effect ", 10, 0.01));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }

    void Effect::destroyFire(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/fire/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/fire/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("f0.png");
        sprite->setScale(0.6* Common::scaleSizeXY());
        sprite->setPosition(position);
        auto animate = Animate::create(Common::createAnimation("f", 13, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }
    void Effect::smoke(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/smoke/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/smoke/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("smoke_0.png");
        sprite->setScale(0.6* Common::scaleSizeXY());
        sprite->setPosition(position);
        auto animate = Animate::create(Common::createAnimation("smoke_", 9, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };


        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }

    void Effect::destroyFireRain(b2World* world, Scene* scene, Vec2 position) {
        auto  spriteNode = SpriteBatchNode::create("Effect/fire_rain/sprites.png");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effect/fire_rain/sprites.plist");
        auto sprite = Sprite::createWithSpriteFrameName("destroy_fire_rain_0.png");
        sprite->setScale(0.6* Common::scaleSizeXY());
        sprite->setPosition(position);
        auto animate = Animate::create(Common::createAnimation("destroy_fire_rain_", 8, 0.02));
        spriteNode->addChild(sprite);
        scene->addChild(spriteNode);

        auto callback = [sprite]() {
            sprite->removeFromParentAndCleanup(true);
            };

        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(animate, callFunc, nullptr);
        sprite->runAction(sequence);
    }

    void Effect::eagle() {
        Setting settingInit;
        settingInit.loadSettingData();
        AudioEngine::setVolume(AudioEngine::play2d("player/sound/eagle.mp3"), 0.2 * settingInit.getGameplayVol());
    }

    void Effect::getStick() {
        Setting settingInit;
        settingInit.loadSettingData();
        AudioEngine::setVolume(AudioEngine::play2d("sound/get_stick.mp3"), 2 * settingInit.getGameplayVol());
    }
}