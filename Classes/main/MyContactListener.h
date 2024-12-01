#pragma once
#ifndef __MyContactListener_H__
#define __MyContactListener_H__

#include "cocos2d.h"
#include <box2d/box2d.h>
#include <vector>
#include "main/Common.h"
#include "enemy/Warrior.h"
#include "enemy/Hedgehog.h"
#include "player/Player.h"
#include "enemy/Acher.h"
#include "enemy/BossMap1.h"
#include "enemy/BossMap2.h"
#include "enemy/BossMap3.h"
#include "main/Effect.h"
#include "enemy/Golem.h"
#include "enemy/Bat.h"
#include "main/Constants.h"
#include "skill/StoneBall.h"

using namespace constants;
using namespace common;

class MyContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void removeObject();
    std::set<b2Body*> bodiesToRemove;
    int stick;

    Player* player;
    Scene* scene;
    b2World* world;
    unordered_map<b2Body*, Sprite*>* bodyToSpriteMap;
    

    MyContactListener(Player* player, Scene* scene, b2World* world);
    MyContactListener(Player* player);

    bool isNext = false;
    BossMap1* bossmap1;
    BossMap2* bossmap2;
    BossMap3* bossmap3;
};
#endif // __MyContactListener_H__