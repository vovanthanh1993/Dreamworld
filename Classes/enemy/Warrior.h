#pragma once
#ifndef __Warrior_H__
#define __Warrior_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseEnemy.h"
#include "main/MusicManager.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Warrior:public BaseEnemy
{
protected:
	BasePool<SlashEnemy>* slashEnemyPool = new BasePool<SlashEnemy>();
public:
	Warrior(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hit();
	void die();
	void update(float dt);
	void followPlayer();
	void getDamage(int damage);
	void hurt();
	bool isFollowPlayer = false;
};

#endif // __Warrior_H__