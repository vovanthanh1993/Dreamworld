#pragma once
#ifndef __BossMap1_H__
#define __BossMap1_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Fire.h"
#include "skill/FireRain.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseNode.h"
#include "skill/StoneBall.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap1:public BaseNode
{
private:
	float attackCooldown =2.0f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	int health = 20;
	int count = 1;
	float maxHealth =20;
	cocos2d::Sprite* healthBar; // Sprite cho thanh máu
	cocos2d::Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
	float scale = 1;
	float attackRange = 1000;
	std::vector<StoneBall*> stoneBallVector;
	void followPlayer();
public:
	bool init();
	void walk();
	void idle();
	void die();
	void throwStoneBall();
	void update(float dt);
	void setHealth(int h);
	int getHealth();
	void charge();
	void updateHealth(int damage);
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	void hurt();
	BossMap1(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	Player* player;
	bool isALive = true;
	int direction = -1;
	int speed = 10;
	void createStoneBall();
	
};

#endif // __BossMap1_H__