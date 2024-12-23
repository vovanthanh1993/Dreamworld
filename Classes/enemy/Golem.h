#pragma once
#ifndef __Golem_H__
#define __Golem_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "skill/SlashEnemy.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "base/BaseEnemy.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Golem:public BaseEnemy
{
protected:
	BasePool<SlashEnemy>* slashEnemyPool = new BasePool<SlashEnemy>();
public:
	Golem(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hit();
	void die();
	void hurt();
	void update(float dt);
	Player* player;
	void getDamage(int damage);
};

#endif // __Golem_H__