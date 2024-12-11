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
#include "base/BaseEnemy.h"
#include "skill/StoneBall.h"
#include "skill/StoneBallPool.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap1:public BaseEnemy
{
private:
	int count = 1;
	float maxHealth = 200;
	Sprite* healthBar; // Sprite cho thanh máu
	Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
	vector<StoneBall*> stoneBallVector;
	StoneBallPool* stoneBallPool;
public:
	void followPlayer();
	bool init(Vec2 position);
	void walk();
	void idle();
	void die();
	void throwStoneBall();
	void update(float dt);
	void setHealth(int h);
	int getHealth();
	void charge();
	void getDamage(int damage);
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	void hurt();
	BossMap1(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	void createStoneBall();
};

#endif // __BossMap1_H__