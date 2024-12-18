#pragma once
#ifndef __BossMap2_H__
#define __BossMap2_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/Fire.h"
#include "skill/PoisonRain.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "skill/skull.h"
#include "skill/BoneRain.h"
#include "skill/BoneRainPool.h"
#include "main/Effect.h"
#include "base/BaseEnemy.h"
#include "enemy/Warrior.h"
#include "enemy/WarriorPool.h"
#include <vector>

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap2:public BaseEnemy
{
private:
	
	int count = 1;
	int maxHealth = 200;
	cocos2d::Sprite* healthBar; // Sprite cho thanh máu
	cocos2d::Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
	std::vector<Warrior*> warVector;
	BoneRainPool* boneRainPool;
	WarriorPool* warriorPool;
	int countPhase2 = 1;
	vector<string> initTextVector = {
		"No one can escape from me! No one can escape death!",
		"All that you have loved will eventually turn to dust...",
		"I understand your fear. It is the last emotion you will ever know...",
		"The ones you love are with me. Do you wish to join them?"
	};
	vector<string> throwWarTextVector = {
		"I can hold someone else's life...",
		"I create life, I end life...",
		"Rise and destroy the enemy, my children...",
		"I bring life to you, fight for me..."

	};
	vector<string> dieTextVector = {
		"I will wait for the moment we meet again...",
		"We will meet again, whether you resist or not...",
		"No! No! No"
	};
public:
	BossMap2(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hurt();
	void die();
	void update(float dt);
	void setHealth(int h);
	int getHealth();
	void boneRain();
	void updateHealth(int damage);
	void moveBodyToPoint();
	void moveBodyToInit();
	void throwWarrior();
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	TMXTiledMap* map;
};

#endif // __BossMap2_H__