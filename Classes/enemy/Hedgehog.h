#pragma once
#ifndef __Hedgehog_H__
#define __Hedgehog_H__

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
class Hedgehog :public BaseEnemy
{
protected:
	
public:
	Hedgehog(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hit();
	void die();
	void hurt();
	void update(float dt);
	void getDamage(int damage);
};

#endif // __Hedgehog_H__