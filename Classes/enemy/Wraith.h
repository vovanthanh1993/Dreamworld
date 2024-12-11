#pragma once
#ifndef __Wraith_H__
#define __Wraith_H__

#include "cocos2d.h"
#include "main/Common.h"
#include "player/Player.h"
#include "item/Gem.h"
#include "main/Constants.h"
#include "main/Effect.h"
#include "skill/PoisonRain.h"
#include "skill/PoisonRainPool.h"
#include "base/BaseEnemy.h"

using namespace constants;
using namespace common;
using namespace cocos2d;
class Wraith: public BaseEnemy
{
private:
	
	PoisonRainPool* poisonRainPool;
public:
	Wraith(b2World* world, Scene* scene, unordered_map<b2Body*, Sprite*>* bodyToSpriteMap);
	bool init(Vec2 position);
	void walk();
	void idle();
	void hit();
	void update(float dt);
};
#endif // __Wraith_H__
