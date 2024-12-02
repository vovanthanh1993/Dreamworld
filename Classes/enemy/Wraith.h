#pragma once
#ifndef __Wraith_H__
#define __Wraith_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "skill/PoisonRain.h"
#include "skill/PoisonRainPool.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Wraith: public BaseNode
{
private:
	float attackCooldown = 3.0f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	PoisonRainPool* poisonRainPool;
public:
	Wraith(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void walk();
	void idle();
	void hit();
	void update(float dt);
	Player* player;
};
#endif // __Wraith_H__
