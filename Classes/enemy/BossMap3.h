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
#include "skill/BoneRain.h"
#include "main/Effect.h"
#include "base/BaseNode.h"
#include "enemy/Bat.h"
#include "base/BasePool.h"
#include <vector>
#include "main/MusicManager.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class BossMap3 :public BaseEnemy
{
private:
	int maxHealth = 200;
	Sprite* healthBar; // Sprite cho thanh máu
	Sprite* healthBarBg; // Sprite cho nền thanh máu
	bool isHit = false;
	void spawnBat();

	BasePool<Bat>* batPool = new BasePool<Bat>();

	int lastPoint = 3;
	vector<string> textVector = {
		"You cannot escape from the darkness...",
		"You cannot know what is in the darkness...",
		"What can you do when the darkness envelops everything?",
		"The darkness holds a truth you do not want to face...",
		"What do you see in the darkest place?"
	};
public:
	BossMap3(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hurt();
	void die();
	void update(float dt);
	void setHealth(int h);
	int getHealth();
	void updateHealth(int damage);
	void moveBodyToPoint();
	void updateHealthBar(float health);
	void createHealthBar();
	void updateHealthBarPosition();
	TMXTiledMap* map;
	void castShadow();
};

#endif // __BossMap3_H__