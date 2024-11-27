#pragma once
#ifndef __Elemental_H__
#define __Elemental_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "skill/Rain.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Elemental:public BaseNode
{
private:
	float attackCooldown = 3.0f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	bool isAlive = true;
public:
	Elemental(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void walk();
	void idle();
	void hit();
	void die();
	void update(float dt);
	Player* player;
};

#endif // __Elemental_H__