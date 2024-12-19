#pragma once
#include "cocos2d.h"
#include "main/Common.h"
#include "main/Constants.h"
#include "base/BaseEnemy.h"
#include "main/Effect.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Soul:public BaseEnemy
{
public:
	Soul(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void getDamage(int damage);
	void hurt();
	bool isHit = false;
	void die();
};

