#pragma once
#ifndef __Bat_H__
#define __Bat_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Bat:public BaseNode
{
protected:
	float attackCooldown =0;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	bool isAlive = true;
	float scale = 0.5;
	float attackRange = 20;
	int health = 1;
public:
	Bat(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void idle();
	void die();
	void hurt();
	void walk();
	void update(float dt);
	Player* player;
	int direction = 1;
	int speed = 6;
	void getDamage(int damage);
};

#endif // __Bat_H__