#pragma once
#ifndef __BossMap3_H__
#define __BossMap3_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Fire.h"
#include "skill/PoisonRain.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "skill/skull.h"
#include "skill/BoneRain.h"
#include "main/Effect.h"
#include "base/BaseNode.h"
#include "enemy/Bat.h"
#include "enemy/BatPool.h"
#include <vector>

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap3 :public BaseNode
{
private:
	float attackCooldown = 2.5f;  // Thời gian chờ giữa các đợt tấn công
	float timeSinceLastAttack = 0.0f;  // Thời gian đã trôi qua kể từ lần tấn công cuối cùng
	bool canAttack = false;  // Cờ để xác định liệu kẻ thù có thể tấn công không
	int health = 1;
	int count = 1;
	int maxHealth = 1;
	Sprite* healthBar; // Sprite cho thanh máu
	Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
	float scale = 0.6;
	void spawnBat();
	BatPool* batPool;
public:
	BossMap3(b2World* world, Scene* scene, Vec2 position, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init();
	void walk();
	void idle();
	void hurt();
	void die();
	void update(float dt);
	void setHealth(int h);
	int getHealth();
	bool isALive = true;
	void updateHealth(int damage);
	void moveBodyToPoint();
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	Player* player;
	TMXTiledMap* map;
	int direction = -1;
	int speed = 10;
	int lastPoint = 3;
};

#endif // __BossMap3_H__