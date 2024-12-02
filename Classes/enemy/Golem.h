#pragma once
#ifndef __Golem_H__
#define __Golem_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "skill/SlashEnemyPool.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseNode.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Golem:public BaseNode
{
protected:
	float attackCooldown =2.0f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	bool isAlive = true;
	float scale = 0.35;
	float attackRange = 3;
	int health = 2;
	SlashEnemyPool* slashEnemyPool;
public:
	Golem(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void walk();
	void idle();
	void hit();
	void die();
	void hurt();
	void update(float dt);
	Player* player;
	int direction = -1;
	int speed = 6;
	void getDamage(int damage);
};

#endif // __Golem_H__